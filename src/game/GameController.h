#pragma once
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QString>
#include <QList> // 新增：用于悔棋的历史落子记录（后续扩展）
// 修正路径：GameController与Player同属src/game目录，直接包含即可
#include "Player.h"
#include "Board.h"
// 引入全局配置（棋子类型、游戏状态）
#include "../story/Constants.h"

/**
 * @brief 游戏逻辑中控类
 * 核心职责：
 * 1. 管理游戏全流程：开局（选择模式）→ 回合切换 → 落子处理 → 胜负/平局判定 → 游戏结束；
 * 2. 桥接棋盘逻辑（Board）与表现层（QML）：转发棋盘状态、落子事件、游戏结果；
 * 3. 管理玩家数据（Player）：区分人类/AI 玩家，控制当前行动方；
 * 4. 实现核心功能：人机对战（AI 落子）、悔棋、游戏重置；
 * 5. 与存档模块（SaveManager）联动：支持保存/读取游戏进度（后续扩展）。
 * 设计特点：继承 QObject 支持信号槽，所有 QML 可调用接口均标记 Q_INVOKABLE，属性变更通过 NOTIFY 信号同步 UI。
 */
class GameController : public QObject
{
    Q_OBJECT  // Qt 信号槽/属性系统必须的宏，不可删除

    // 暴露给 QML 的属性：QML 可直接绑定，属性变化时自动触发 NOTIFY 信号更新 UI
    /**
     * @brief 当前行动玩家的名称（如“黑方”“白方”）
     * READ：指定读取函数；NOTIFY：属性变化时发射 turnChanged 信号
     * QML 绑定场景：GameView 中显示“当前回合：XX方”。
     */
    Q_PROPERTY(QString currentPlayerName READ currentPlayerName NOTIFY turnChanged)
    /**
     * @brief 游戏是否结束的状态标记
     * READ：指定读取函数；NOTIFY：属性变化时发射 gameOver 信号
     * QML 绑定场景：GameView 中禁用落子按钮、显示游戏结束弹窗。
     */
    Q_PROPERTY(bool isGameOver READ isGameOver NOTIFY gameOver)

public:
    /**
     * @brief 构造函数（显式构造，禁止隐式转换）
     * @param parent 父对象指针（由 AppController 管理生命周期，避免内存泄漏）
     * 初始化逻辑：
     * 1. 初始化黑白玩家（默认均为人类玩家，人机模式下动态修改白方类型）；
     * 2. 设置黑方为先手玩家；
     * 3. 初始化游戏结束标记为 false；
     * 4. （可选）初始化悔棋历史记录容器。
     */
    explicit GameController(QObject *parent = nullptr);

    /**
     * @brief 开始新游戏（QML 可调用）
     * @param mode 游戏模式：0=人人对战，1=人机对战
     * 功能逻辑：
     * 1. 重置棋盘（调用 Board::reset()）；
     * 2. 重置游戏结束标记为 false；
     * 3. 根据模式设置玩家类型（人机模式下将白方设为 AI）；
     * 4. 重置当前玩家为黑方（先手）；
     * 5. 发射 turnChanged 信号更新 UI；
     * 6. （可选）清空悔棋历史记录。
     */
    Q_INVOKABLE void startGame(int mode);

    /**
     * @brief 处理 QML 落子输入（QML 可调用）
     * @param row 落子行坐标（0~Config::BOARD_SIZE-1）
     * @param col 落子列坐标（0~Config::BOARD_SIZE-1）
     * 核心逻辑：
     * 1. 校验游戏是否已结束（结束则直接返回）；
     * 2. 校验当前玩家是否为人类（AI 玩家则忽略输入）；
     * 3. 调用 Board::placePiece() 执行落子，校验落子合法性；
     * 4. 落子成功后：记录历史落子（悔棋用）、发射 pieceAdded 信号触发 UI 动画；
     * 5. 调用 Board::checkWin() 判断是否获胜，获胜则发射 gameOver 信号；
     * 6. 未获胜则调用 Board::isFull() 判断是否平局，平局则发射 gameOver 信号；
     * 7. 未结束则切换回合（switchTurn()），若新回合是 AI 玩家则触发 AI 落子（processAIMove()）。
     */
    Q_INVOKABLE void handleInput(int row, int col);

    /**
     * @brief 获取棋盘指定位置的棋子状态（QML 可调用）
     * @param row 行坐标
     * @param col 列坐标
     * @return int 棋子状态编码：0=空（PieceType::None），1=黑棋（PieceType::Black），2=白棋（PieceType::White）
     * QML 调用场景：GameView 初始化/刷新时，遍历棋盘生成棋子组件。
     */
    Q_INVOKABLE int getBoardState(int row, int col);

    /**
     * @brief 悔棋功能（QML 可调用）
     * 功能逻辑：
     * 1. 校验游戏是否已结束（结束则无法悔棋）；
     * 2. 校验历史落子记录是否为空（无记录则无法悔棋）；
     * 3. 回退上一步落子（从 Board 中清除棋子，删除历史记录最后一项）；
     * 4. 切换回上一玩家（再次调用 switchTurn()）；
     * 5. 发射 pieceAdded 信号（传空棋子类型）通知 UI 移除棋子；
     * 6. （人机模式）若回退后是 AI 玩家，需取消未执行的 AI 落子。
     */
    Q_INVOKABLE void undo();

    /**
     * @brief Q_PROPERTY 对应的 READ 函数：获取当前玩家名称
     * @return QString 当前玩家名称（如“黑方”“白方”，若指针为空则返回空字符串）
     * @note const 修饰：函数不修改类成员变量，仅做查询。
     */
    QString currentPlayerName() const { return m_currentPlayer ? m_currentPlayer->name() : ""; }

    /**
     * @brief Q_PROPERTY 对应的 READ 函数：获取游戏结束状态
     * @return bool 游戏是否结束
     * @note const 修饰：函数不修改类成员变量，仅做查询。
     */
    bool isGameOver() const { return m_isGameOver; }

signals:
    /**
     * @brief 回合切换信号（NOTIFY 信号）
     * 触发时机：调用 switchTurn() 后、startGame() 初始化回合后。
     * QML 响应逻辑：更新“当前回合”文本、切换玩家标识样式。
     */
    void turnChanged();

    /**
     * @brief 落子成功信号（通知 QML 渲染棋子）
     * @param row 落子行坐标
     * @param col 落子列坐标
     * @param type 棋子类型编码（0=空，1=黑，2=白）
     * QML 响应逻辑：在对应位置生成 ChessPiece 组件、播放落子动画。
     */
    void pieceAdded(int row, int col, int type);

    /**
     * @brief 游戏结束信号（NOTIFY 信号）
     * @param winnerName 获胜者名称（平局则传“平局”）
     * QML 响应逻辑：显示游戏结束弹窗、播放胜利/平局音效、提供“再来一局”按钮。
     */
    void gameOver(QString winnerName);

private:
    /**
     * @brief 切换当前行动玩家（私有辅助函数）
     * 逻辑：将 m_currentPlayer 在黑方与白方之间切换，切换后发射 turnChanged 信号。
     */
    void switchTurn();

    /**
     * @brief 处理 AI 落子逻辑（私有辅助函数）
     * 核心逻辑：
     * 1. 校验当前玩家是否为 AI（非 AI 则直接返回）；
     * 2. AI 算法计算最优落子位置（简单版：随机空位置；进阶版：极大极小值/蒙特卡洛算法）；
     * 3. 调用 handleInput() 执行 AI 落子（模拟人类输入）；
     * 4. （可选）添加 AI 思考延迟（用 QTimer 实现，模拟真实思考过程）。
     */
    void processAIMove();

    // 私有成员变量
    /**
     * @brief 棋盘实例（核心逻辑依赖，负责落子校验、胜负判断）
     * 说明：直接实例化而非指针，利用栈内存管理，无需手动释放。
     */
    Board m_board;

    /**
     * @brief 黑棋玩家实例（固定为黑棋，先手）
     */
    Player m_blackPlayer;

    /**
     * @brief 白棋玩家实例（人机模式下动态改为 AI 类型）
     */
    Player m_whitePlayer;

    /**
     * @brief 当前行动玩家指针（指向 m_blackPlayer 或 m_whitePlayer）
     * 初始化：设为黑方（先手），避免野指针。
     */
    Player* m_currentPlayer = nullptr;

    /**
     * @brief 游戏结束状态标记
     * 初始值：false（游戏未开始/进行中）；获胜/平局后设为 true。
     */
    bool m_isGameOver = false;

    /**
     * @brief 落子历史记录（用于悔棋功能，后续扩展）
     * 存储格式：QList<QPair<int, int>>，每个元素是（行，列）的落子坐标。
     */
    QList<QPair<int, int>> m_moveHistory;
};

#endif // GAMECONTROLLER_H
