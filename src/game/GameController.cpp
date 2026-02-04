#include "GameController.h"
#include <QDebug>       // 调试日志打印
#include <QTimer>       // AI 思考延迟
#include <QRandomGenerator> // 更好的随机数
#include <algorithm>
#include <limits>
#include "../story/Constants.h" // 全局配置（棋子类型、棋盘大小）
//为实现AI落子 搞了这个namespace 上面也引进了一些头文件
namespace {

struct CandidateMove
{
    int row = -1;
    int col = -1;
    int score = std::numeric_limits<int>::min();
};

inline bool inBounds(int r, int c)
{
    return r >= 0 && r < Config::BOARD_SIZE && c >= 0 && c < Config::BOARD_SIZE;
}

inline int centerBonus(int r, int c)
{
    // 越靠近中心加分越多（范围大概 0~28）
    const int center = Config::BOARD_SIZE / 2;
    const int dist = std::abs(r - center) + std::abs(c - center);
    return (Config::BOARD_SIZE * 2) - dist;
}

Config::PieceType opponentOf(Config::PieceType self)
{
    return (self == Config::PieceType::Black) ? Config::PieceType::White : Config::PieceType::Black;
}

struct LineInfo
{
    int count = 1;      // 以 (row,col) 为落子点（假设已落子）的同色连续数
    int openEnds = 0;   // 两端空位数：0/1/2
};

LineInfo analyzeLineAssumingPlaced(const Board& board, int row, int col, Config::PieceType type, int dr, int dc)
{
    // 统计从 (row,col) 向 (dr,dc) 与 (-dr,-dc) 两侧的连续同色棋子数，并判断两端是否为空
    LineInfo info;

    // 正向
    int r = row + dr;
    int c = col + dc;
    while (inBounds(r, c) && board.getPiece(r, c) == type)
    {
        ++info.count;
        r += dr;
        c += dc;
    }
    if (inBounds(r, c) && board.getPiece(r, c) == Config::PieceType::None)
    {
        ++info.openEnds;
    }

    // 反向
    r = row - dr;
    c = col - dc;
    while (inBounds(r, c) && board.getPiece(r, c) == type)
    {
        ++info.count;
        r -= dr;
        c -= dc;
    }
    if (inBounds(r, c) && board.getPiece(r, c) == Config::PieceType::None)
    {
        ++info.openEnds;
    }

    return info;
}

int scoreLine(const LineInfo& li)
{
    // 经典五子棋启发式：连子越长、两端越活，分越高
    if (li.count >= 5) return 1'000'000;                // 连五（理论上一步就赢）
    if (li.count == 4 && li.openEnds == 2) return 120'000; // 活四
    if (li.count == 4 && li.openEnds == 1) return 25'000;  // 冲四
    if (li.count == 3 && li.openEnds == 2) return 6'000;   // 活三
    if (li.count == 3 && li.openEnds == 1) return 1'200;   // 眠三
    if (li.count == 2 && li.openEnds == 2) return 400;     // 活二
    if (li.count == 2 && li.openEnds == 1) return 120;     // 眠二
    if (li.count == 1 && li.openEnds == 2) return 20;      // 活一
    return 0;
}

int scorePointAssumingPlaced(const Board& board, int row, int col, Config::PieceType type)
{
    // 若该点非空，则不可落子
    if (board.getPiece(row, col) != Config::PieceType::None)
    {
        return std::numeric_limits<int>::min() / 4;
    }
    // 四个方向：横、竖、两条对角线
    static constexpr int dirs[4][2] =
    {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };
    int bestDir = 0;
    int sum = 0;
    for (const auto& d : dirs)
    {
        const LineInfo li = analyzeLineAssumingPlaced(board, row, col, type, d[0], d[1]);
        const int s = scoreLine(li);
        sum += s;
        bestDir = std::max(bestDir, s);
    }
    // 综合评分：方向最强形状为主，其余方向小权重叠加（鼓励双向威胁）
    return bestDir * 5 + sum + centerBonus(row, col);
}

QList<QPair<int, int>> generateCandidatesNearStones(const Board& board, int radius)
{
    // 只从已有棋子附近生成候选点（性能更好，也避免“乱下角落”）
    bool hasStone = false;
    for (int r = 0; r < Config::BOARD_SIZE && !hasStone; ++r)
    {
        for (int c = 0; c < Config::BOARD_SIZE; ++c)
        {
            if (board.getPiece(r, c) != Config::PieceType::None)
            {
                hasStone = true;
                break;
            }
        }
    }
    QList<QPair<int, int>> result;
    if (!hasStone)
    {
        // 空棋盘：直接下中心
        result.append({Config::BOARD_SIZE / 2, Config::BOARD_SIZE / 2});
        return result;
    }
    // 标记去重
    bool mark[Config::BOARD_SIZE][Config::BOARD_SIZE] = {};
    for (int r = 0; r < Config::BOARD_SIZE; ++r)
    {
        for (int c = 0; c < Config::BOARD_SIZE; ++c)
        {
            if (board.getPiece(r, c) == Config::PieceType::None) continue;
            for (int dr = -radius; dr <= radius; ++dr)
            {
                for (int dc = -radius; dc <= radius; ++dc)
                {
                    const int nr = r + dr;
                    const int nc = c + dc;
                    if (!inBounds(nr, nc)) continue;
                    if (board.getPiece(nr, nc) != Config::PieceType::None) continue;
                    if (!mark[nr][nc])
                    {
                        mark[nr][nc] = true;
                        result.append({nr, nc});
                    }
                }
            }
        }
    }
    return result;
}
} // namespace

/**
 * @brief 构造函数实现：初始化玩家与游戏状态
 * 详细实现逻辑：
 * 1. 调用父类 QObject 构造函数，绑定父对象；
 * 2. 初始化黑棋玩家：名称“黑方”，棋子类型 Black，默认人类玩家；
 * 3. 初始化白棋玩家：名称“白方”，棋子类型 White，默认人类玩家（人机模式下动态修改）；
 * 4. 设置当前玩家为黑方（五子棋规则：黑方先手）；
 * 5. 初始化游戏结束标记为 false；
 * 6. （可选）清空落子历史记录（m_moveHistory.clear()）；
 * 7. 打印初始化日志，便于调试。
 * @param parent 父对象指针（由 AppController 传入）
 */
GameController::GameController(QObject *parent)
    : QObject(parent)
    // 初始化黑白玩家：参数依次为【名称、棋子类型、玩家类型】
    , m_blackPlayer("黑方", Config::PieceType::Black, Player::Type::Human)
    , m_whitePlayer("白方", Config::PieceType::White, Player::Type::Human)
    , m_currentPlayer(&m_blackPlayer) // 黑方先手
    , m_isGameOver(false)
{
    // 初始化落子历史记录（可选，悔棋功能用）
    m_moveHistory.clear();
    qInfo() << "[GameController] 初始化完成，默认黑方先手";
}

/**
 * @brief 开始新游戏函数实现
 * 详细实现逻辑（组员需补充完善）：
 * Step1：重置棋盘状态
 * - 调用 m_board.reset()，将棋盘所有位置恢复为空。
 * Step2：重置游戏状态与玩家类型
 * - 将 m_isGameOver 设为 false；
 * - 根据 mode 参数设置玩家类型：mode=1 时，将 m_whitePlayer 的类型改为 Player::Type::AI；
 * - 重置当前玩家为黑方（&m_blackPlayer）。
 * Step3：同步 UI 与日志
 * - 发射 turnChanged() 信号，通知 QML 更新当前玩家显示；
 * - 打印游戏模式日志，便于调试。
 * Step4：清空历史记录（悔棋用）
 * - 调用 m_moveHistory.clear()。
 * @param mode 游戏模式：0=人人对战，1=人机对战
 */
void GameController::startGame(int mode)
{
    // 重置棋盘与状态
    m_board.reset();
    m_isGameOver = false;
    // 根据模式设置玩家类型：0=人人，1=人机（白方为 AI，默认困难难度）
    m_blackPlayer.setType(Player::Type::Human);
    if (mode == 1)
    {
        m_whitePlayer.setType(Player::Type::AI_Hard);
    }
    else
    {
        m_whitePlayer.setType(Player::Type::Human);
    }
    // 清空历史记录（用于悔棋）
    m_moveHistory.clear();
    // 黑方先手
    m_currentPlayer = &m_blackPlayer;
    emit turnChanged(); // 发送换手信号，更新 UI 显示
    qInfo() << "[GameController] 游戏开始，模式：" << (mode == 0 ? "人人对战" : "人机对战");
}

/**
 * @brief 处理 QML 落子输入函数实现
 * 详细实现逻辑（组员需补充核心逻辑）：
 * Step1：前置校验（快速失败）
 * - 若 m_isGameOver 为 true，打印警告并返回（游戏已结束，禁止落子）；
 * - 若当前玩家是 AI，打印警告并返回（AI 落子由 processAIMove 处理）；
 * - 调用 m_board.placePiece() 执行落子，传入（row, col, 当前玩家棋子类型），接收落子结果。
 * Step2：落子失败处理
 * - 若 placePiece 返回 false，打印“落子失败（位置越界/已有棋子）”日志并返回。
 * Step3：落子成功处理
 * - 记录落子坐标到历史记录：m_moveHistory.append(QPair<int, int>(row, col));
 * - 转换棋子类型为编码（None=0, Black=1, White=2），发射 pieceAdded 信号通知 QML 渲染棋子；
 * - 播放落子音效（组员补充：调用 ResourceManager::instance().playSound("drop_chess.wav")）。
 * Step4：胜负与平局判定
 * - 调用 m_board.checkWin(row, col, m_currentPlayer->pieceType()) 判断是否获胜：
 *   → 获胜：设置 m_isGameOver = true，发射 gameOver(m_currentPlayer->name()) 信号，返回；
 * - 调用 m_board.isFull() 判断是否平局：
 *   → 平局：设置 m_isGameOver = true，发射 gameOver("平局") 信号，返回；
 * Step5：切换回合与 AI 落子
 * - 调用 switchTurn() 切换玩家；
 * - 若新的当前玩家是 AI，调用 processAIMove() 触发 AI 落子。
 * @param row 落子行坐标
 * @param col 落子列坐标
 */
void GameController::handleInput(int row, int col)
{
    qInfo() << "[GameController] 收到落子输入：行" << row << "列" << col;
    // 前置校验：游戏是否结束
    if (m_isGameOver)
    {
        qWarning() << "[GameController] 游戏已结束，忽略落子输入";
        return;
    }
    // 前置校验：当前玩家是否为人类
    if (m_currentPlayer && m_currentPlayer->isAI())
    {
        qWarning() << "[GameController] 当前为 AI 回合，忽略手动输入";
        return;
    }
    // 落子合法性校验与执行
    Config::PieceType pieceType = m_currentPlayer ? m_currentPlayer->color() : Config::PieceType::None;
    if (!m_board.placePiece(row, col, pieceType))
    {
        qWarning() << "[GameController] 落子失败：坐标非法或已有棋子，行" << row << "列" << col;
        return;
    }
    // 记录历史落子（用于悔棋）
    m_moveHistory.append(QPair<int, int>(row, col));
    // 将棋子类型转换为 QML 使用的 int 编码
    int typeCode = 0;
    switch (pieceType)
    {
    case Config::PieceType::Black:
        typeCode = 1;
        break;
    case Config::PieceType::White:
        typeCode = 2;
        break;
    default:
        typeCode = 0;
        break;
    }
    // 通知 QML 渲染棋子
    emit pieceAdded(row, col, typeCode);
    // 胜负判断
    if (m_board.checkWin(row, col, pieceType))
    {
        m_isGameOver = true;
        const QString winnerName = m_currentPlayer ? m_currentPlayer->name() : QStringLiteral("未知");
        emit gameOver(winnerName);
        qInfo() << "[GameController] 游戏结束，获胜方：" << winnerName;
        return;
    }
    // 平局判断
    if (m_board.isFull())
    {
        m_isGameOver = true;
        emit gameOver(QStringLiteral("平局"));
        qInfo() << "[GameController] 游戏结束：平局";
        return;
    }
    // 切换回合
    switchTurn();
    // 若新回合为 AI，则触发 AI 落子
    if (m_currentPlayer && m_currentPlayer->isAI())
    {
        processAIMove();
    }
}

/**
 * @brief 获取棋盘状态函数实现
 * 详细实现逻辑（组员需补充）：
 * Step1：调用 m_board.getPiece(row, col) 获取指定位置的棋子类型；
 * Step2：将 PieceType 枚举转换为 int 编码（便于 QML 处理）：
 *   → PieceType::None → 0，PieceType::Black → 1，PieceType::White → 2；
 * Step3：返回转换后的编码。
 * @param row 行坐标
 * @param col 列坐标
 * @return int 棋子状态编码
 */
int GameController::getBoardState(int row, int col)
{
    Config::PieceType pieceType = m_board.getPiece(row, col);
    switch (pieceType)
    {
    case Config::PieceType::Black:
        return 1;
    case Config::PieceType::White:
        return 2;
    case Config::PieceType::None:
    default:
        return 0;
    }
}

/**
 * @brief 悔棋功能实现
 * 详细实现逻辑（组员需补充）：
 * Step1：前置校验
 * - 若 m_isGameOver 为 true，打印警告并返回（游戏结束无法悔棋）；
 * - 若 m_moveHistory 为空，打印警告并返回（无落子记录无法悔棋）；
 * Step2：回退上一步落子
 * - 获取最后一次落子的坐标：QPair<int, int> lastMove = m_moveHistory.takeLast();
 * - 调用 m_board.placePiece(lastMove.first, lastMove.second, Config::PieceType::None)（清除棋子）；
 * - 发射 pieceAdded 信号，传入（lastMove.first, lastMove.second, 0）通知 QML 移除棋子；
 * Step3：切换回上一玩家
 * - 调用 switchTurn()（因为上一步切换过，再次调用即可回退）；
 * Step4：人机模式特殊处理
 * - 若回退后的当前玩家是 AI，需取消未执行的 AI 落子（如停止 QTimer 延迟）。
 */
void GameController::undo()
{
    qInfo() << "[GameController] 执行悔棋操作";
    // 游戏结束无法悔棋
    if (m_isGameOver)
    {
        qWarning() << "[GameController] 游戏已结束，无法悔棋";
        return;
    }
    // 无历史记录无法悔棋
    if (m_moveHistory.isEmpty())
    {
        qWarning() << "[GameController] 无落子记录，无法悔棋";
        return;
    }
    // 取出最后一步落子
    QPair<int, int> lastMove = m_moveHistory.takeLast();
    int row = lastMove.first;
    int col = lastMove.second;
    // 从棋盘上清除该棋子
    if (!m_board.placePiece(row, col, Config::PieceType::None))
    {
        qWarning() << "[GameController] 悔棋失败：无法清除棋子，行" << row << "列" << col;
        return;
    }
    // 通知 QML 移除棋子（type=0 表示空）
    emit pieceAdded(row, col, 0);
    // 切回上一玩家
    switchTurn();
    // 人机模式下，如需更复杂处理（例如取消未触发的 AI 计时器），可在此扩展
}

/**
 * @brief 切换回合函数实现
 * 详细实现逻辑（已完成核心，需补充日志）：
 * 1. 切换当前玩家指针：黑方↔白方；
 * 2. 发射 turnChanged 信号，通知 QML 更新当前玩家显示；
 * 3. 打印回合切换日志，便于调试。
 */
void GameController::switchTurn()
{
    // 核心逻辑已完成，补充日志与注释
    if (m_currentPlayer == &m_blackPlayer)
    {
        m_currentPlayer = &m_whitePlayer;
    }
    else
    {
        m_currentPlayer = &m_blackPlayer;
    }
    emit turnChanged(); // 发送信号更新 UI
    qInfo() << "[GameController] 回合切换，当前玩家：" << m_currentPlayer->name();
}

/**
 * @brief AI 落子逻辑实现
 * 详细实现逻辑（组员需补充）：
 * 方案一：简单随机落子（适合初期测试）
 * Step1：生成随机坐标（row: 0~14，col: 0~14）；
 * Step2：循环校验坐标是否为空（调用 m_board.getPiece(row, col) == PieceType::None）；
 * Step3：找到空位置后，延迟 500ms 调用 handleInput(row, col)（用 QTimer::singleShot 实现延迟）；
 * 方案二：进阶 AI 落子（极大极小值算法）
 * Step1：遍历棋盘所有空位置，计算每个位置的得分（进攻/防守权重）；
 * Step2：选择得分最高的位置作为落子点；
 * Step3：延迟调用 handleInput 执行落子。
 * 注意：添加延迟是为了模拟 AI“思考”的过程，提升用户体验。
 */
void GameController::processAIMove()//AI写的 我想到的是1.0版本的那种 但是比下面的这个低级很多
{
    qInfo() << "[GameController] AI 正在思考落子...";
    // 若当前不是 AI 玩家或游戏已结束，则不处理
    if (!m_currentPlayer || !m_currentPlayer->isAI() || m_isGameOver)
    {
        return;
    }
    const Config::PieceType aiType = m_currentPlayer->color();
    const Config::PieceType oppType = opponentOf(aiType);
    //候选点:围绕已有棋子2格范围
    QList<QPair<int, int>> candidates = generateCandidatesNearStones(m_board, /*radius*/ 2);
    if (candidates.isEmpty())
    {
        qWarning() << "[GameController] AI 无可落子位置";
        return;
    }
    // 1) 必胜:下这里立刻赢
    for (const auto& p : candidates)
    {
        const int r = p.first;
        const int c = p.second;
        if (m_board.getPiece(r, c) != Config::PieceType::None) continue;
        m_board.placePiece(r, c, aiType);
        const bool win = m_board.checkWin(r, c, aiType);
        m_board.placePiece(r, c, Config::PieceType::None);
        if (win)
        {
            QTimer::singleShot(400, this, [this, r, c]() { this->handleInput(r, c); });
            return;
        }
    }
    // 2) 必挡：不挡对手就下一手赢
    for (const auto& p : candidates)
    {
        const int r = p.first;
        const int c = p.second;
        if (m_board.getPiece(r, c) != Config::PieceType::None) continue;
        m_board.placePiece(r, c, oppType);
        const bool oppWin = m_board.checkWin(r, c, oppType);
        m_board.placePiece(r, c, Config::PieceType::None);
        if (oppWin)
        {
            QTimer::singleShot(400, this, [this, r, c]() { this->handleInput(r, c); });
            return;
        }
    }
    // 3) 评分选点：进攻 + 防守（防守略高） + 中心偏好
    CandidateMove best;
    QList<CandidateMove> scored;
    scored.reserve(candidates.size());
    const bool isHard = (m_currentPlayer->type() == Player::Type::AI_Hard);
    const int defenseWeightTimes100 = isHard ? 125 : 115; // 1.25 / 1.15
    for (const auto& p : candidates)
    {
        const int r = p.first;
        const int c = p.second;
        if (m_board.getPiece(r, c) != Config::PieceType::None) continue;
        // 形状评分（假设落子）
        const int attack = scorePointAssumingPlaced(m_board, r, c, aiType);
        const int defend = scorePointAssumingPlaced(m_board, r, c, oppType);
        int total = attack + (defend * defenseWeightTimes100) / 100;
        // 困难AI：额外偏好“同时制造双向威胁”的点（attack 已含 sum，但这里再小加成）
        if (isHard)
        {
            total += (attack / 50);
        }
        CandidateMove cm{r, c, total};
        scored.append(cm);
        if (cm.score > best.score) best = cm;
    }
    if (best.row < 0)
    {
        qWarning() << "[GameController] AI 无可落子位置";
        return;
    }
    // 简单AI：在接近最优的一小批点中随机，避免“每局都一样”
    int finalRow = best.row;
    int finalCol = best.col;
    if (!isHard && !scored.isEmpty())
    {
        std::sort(scored.begin(), scored.end(), [](const CandidateMove& a, const CandidateMove& b)
        {
            return a.score > b.score;
        });
        const int topK = std::min<int>(5, scored.size());
        const int pick = QRandomGenerator::global()->bounded(topK);
        finalRow = scored[pick].row;
        finalCol = scored[pick].col;
    }
    QTimer::singleShot(500, this, [this, finalRow, finalCol]()
    {
        this->handleInput(finalRow, finalCol);
    });
}
