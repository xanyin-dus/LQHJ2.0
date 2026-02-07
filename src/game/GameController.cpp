#include "GameController.h"
#include <QDebug>       // 调试日志打印
#include <QTimer>       // AI 思考延迟
#include <cstdlib>      // AI 随机落子的随机数生成
#include "../story/Constants.h" // 全局配置（棋子类型、棋盘大小）

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
    // 基础空实现（已完成部分逻辑，需补充）
    m_isGameOver = false;
    // 重置棋盘（组员补充：m_board.reset();）
    // 重置玩家类型（组员补充：人机模式下修改白方类型）
    m_currentPlayer = &m_blackPlayer; // 黑方先手
    emit turnChanged(); // 发送换手信号，更新 UI 显示
    // 清空落子历史（组员补充：m_moveHistory.clear();）
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
    // 基础空实现（需补充核心逻辑）
    qInfo() << "[GameController] 收到落子输入：行" << row << "列" << col;

    // 组员补充：前置校验（游戏是否结束、当前玩家是否为人类）
    // 组员补充：落子合法性校验（m_board.placePiece）
    // 组员补充：落子成功后的历史记录、信号发射、胜负判断
    // 组员补充：切换回合与 AI 落子触发
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
    // 基础空实现（需补充转换逻辑）
    Config::PieceType pieceType = m_board.getPiece(row, col);
    // 组员补充：将 pieceType 转换为 int 编码并返回
    return 0; // 临时返回 0（空），需修改
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
    // 基础空实现（需补充核心逻辑）
    qInfo() << "[GameController] 执行悔棋操作";
    // 组员补充：前置校验、回退落子、切换玩家、通知 UI
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
    if (m_currentPlayer == &m_blackPlayer) {
        m_currentPlayer = &m_whitePlayer;
    } else {
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
void GameController::processAIMove()
{
    // 基础空实现（需补充核心逻辑）
    qInfo() << "[GameController] AI 正在思考落子...";
    // 组员补充：AI 算法计算落子位置
    // 组员补充：用 QTimer 延迟触发 handleInput 执行落子
}
