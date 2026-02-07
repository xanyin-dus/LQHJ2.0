#include "Board.h"

/**
 * @brief 构造函数实现：初始化棋盘为空
 * 详细实现逻辑（组员需补充）：
 * 遍历15×15的m_grid数组，将所有元素初始化为Config::PieceType::None。
 */
Board::Board() {
    // 空实现：组员需补充初始化逻辑
}

/**
 * @brief 重置棋盘实现
 * 详细实现逻辑（组员需补充）：
 * 遍历m_grid数组，将所有位置重置为Config::PieceType::None，恢复初始状态。
 */
void Board::reset() {
    // 空实现：组员需补充重置逻辑
}

/**
 * @brief 落子操作实现
 * 详细实现逻辑（组员需补充）：
 * Step1：合法性校验
 * - 检查row和col是否在0~Config::BOARD_SIZE-1范围内（越界则返回false）；
 * - 检查目标位置当前是否为PieceType::None（已有棋子则返回false）。
 * Step2：执行落子
 * - 将m_grid[row][col]设置为传入的type。
 * Step3：返回结果
 * - 落子成功返回true，失败返回false。
 * @param row 目标行坐标
 * @param col 目标列坐标
 * @param type 棋子类型
 * @return bool 落子结果
 */
bool Board::placePiece(int row, int col, Config::PieceType type) {
    // 空实现：返回false，组员需补充逻辑
    return false;
}

/**
 * @brief 获取棋子类型实现
 * 详细实现逻辑（组员需补充）：
 * Step1：合法性校验
 * - 若row/col越界，返回PieceType::None；
 * Step2：返回结果
 * - 返回m_grid[row][col]的棋子类型。
 * @param row 行坐标
 * @param col 列坐标
 * @return Config::PieceType 棋子类型
 */
Config::PieceType Board::getPiece(int row, int col) const {
    // 空实现：返回None，组员需补充逻辑
    return Config::PieceType::None;
}

/**
 * @brief 胜负判断实现
 * 详细实现逻辑（组员需补充）：
 * 核心思路：检查落子点的4个方向（横、竖、斜），统计连续同色棋子数量，任意方向≥5则获胜。
 * Step1：定义4个方向的偏移量（dx, dy）：
 * - 水平方向：(0, 1)、(0, -1)
 * - 垂直方向：(1, 0)、(-1, 0)
 * - 左上→右下：(1, 1)、(-1, -1)
 * - 右上→左下：(1, -1)、(-1, 1)
 * Step2：对每个方向，向两侧扩展统计连续同色棋子数量
 * - 例如水平方向：从落子点向右数连续同色棋子数 + 向左数连续同色棋子数 + 1（当前落子）≥5 → 获胜。
 * Step3：若任意方向满足条件，返回true；否则返回false。
 * @param row 落子行坐标
 * @param col 落子列坐标
 * @param type 棋子类型
 * @return bool 胜负结果
 */
bool Board::checkWin(int row, int col, Config::PieceType type) {
    // 空实现：返回false，组员需补充判断逻辑
    return false;
}

/**
 * @brief 棋盘满状态检查实现
 * 详细实现逻辑（组员需补充）：
 * Step1：遍历整个m_grid数组
 * - 若发现任意位置为PieceType::None，返回false（棋盘未满）；
 * Step2：遍历完成后返回true（所有位置均有棋子，平局）。
 * @return bool 棋盘满状态
 */
bool Board::isFull() const {
    // 空实现：返回false，组员需补充检查逻辑
    return false;
}
