#include "Board.h"
//星子:能用前置++的就不要用后置++ 否则编译会比较复杂
//有用auto 毕竟是C++11以上的版本了
//用的标点符号是英文(感觉看着舒服一点)
//有static 没有assert
/**
 * @brief 构造函数实现：初始化棋盘为空
 * 详细实现逻辑（组员需补充）：
 * 遍历15×15的m_grid数组，将所有元素初始化为Config::PieceType::None。
 */
Board::Board()
{
    //两层for全遍历棋盘，将所有位置初始化为空
    for (int r = 0; r < Config::BOARD_SIZE; ++r)
    {
        for (int c = 0; c < Config::BOARD_SIZE; ++c)
        {
            m_grid[r][c] = Config::PieceType::None;
        }
    }
}

/**
 * @brief 重置棋盘实现
 * 详细实现逻辑（组员需补充）：
 * 遍历m_grid数组，将所有位置重置为Config::PieceType::None，恢复初始状态。
 */
void Board::reset()
{
    //和上面的构造函数逻辑一致：全部重置为空 cv即可
    for (int r = 0; r < Config::BOARD_SIZE; ++r)
    {
        for (int c = 0; c < Config::BOARD_SIZE; ++c)
        {
            m_grid[r][c] = Config::PieceType::None;
        }
    }
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
bool Board::placePiece(int row, int col, Config::PieceType type)
{
    //合法性校验:越界检查
    if (row < 0 || row >= Config::BOARD_SIZE ||  col < 0 || col >= Config::BOARD_SIZE)
    {
        return false;
    }
    //特殊功能的实现:当传入 PieceType::None时,用于"清除棋子"(悔棋逻辑)
    if (type == Config::PieceType::None)
    {
        m_grid[row][col] = Config::PieceType::None;
        return true;
    }
    //合法性校验:位置已被占用,落子失败
    if (m_grid[row][col] != Config::PieceType::None)
    {
        return false;
    }
    //合法即可执行落子
    m_grid[row][col] = type;
    return true;
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
Config::PieceType Board::getPiece(int row, int col) const
{
    //合法性校验
    if (row < 0 || row >= Config::BOARD_SIZE || col < 0 || col >= Config::BOARD_SIZE)
    {
        return Config::PieceType::None;
    }
    return m_grid[row][col];
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
bool Board::checkWin(int row, int col, Config::PieceType type)//传进来的是坐标可以借鉴之前的isWin函数
{
    // 空棋检测：如果下的是空棋，直接返回false
    if (type == Config::PieceType::None)
    {
        return false;
    }
    //四个方向:水平 垂直 左上~右下 右上~左下
    //注意:每个方向只需要定义正向 反向通过取负即可得到
    const int directions[4][2] =  //四个方向向量 后面会考 嘿嘿
        {
            {0, 1},   //水平方向(右)
            {1, 0},   //垂直方向(下)
            {1, 1},   //左上~右下对角线
            {1, -1}   //右上~左下对角线
        };//这个数组这样换行看得比较直观
    //遍历四个方向进行检查 前方高能 很nb的方法
    for (const auto &dir : directions)
    {
        int dx = dir[0];  //行方向增量 横
        int dy = dir[1];  //列方向增量 竖
        int count = 1;    //计数从1开始(刚下的这颗子)
        //正向统计(沿方向向量)
        int r = row + dx;
        int c = col + dy;
        //边界检查and同色棋子检查
        while (r >= 0 && r < Config::BOARD_SIZE && c >= 0 && c < Config::BOARD_SIZE && m_grid[r][c] == type)
        {
            ++count;//找到连续同色棋子,计数+1
            r += dx;//继续向正方向移动
            c += dy;
        }
        //反向统计(沿方向向量反方向)
        r = row - dx;
        c = col - dy;
        //边界检查and同色棋子检查
        while (r >= 0 && r < Config::BOARD_SIZE && c >= 0 && c < Config::BOARD_SIZE && m_grid[r][c] == type)
        {
            ++count;//找到连续同色棋子 计数+1
            r -= dx;//继续向反方向移动
            c -= dy;
        }
        //检查是否满足胜利条件(五子连珠)
        if (count >= 5)//这里不要写5 最大可能有9子连珠
        {
            return true;//当前方向满足
        }
    }
    //所有方向都不到五颗
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
bool Board::isFull() const
{
    //全遍历
    for (int r = 0; r < Config::BOARD_SIZE; ++r)
    {
        for (int c = 0; c < Config::BOARD_SIZE; ++c)
        {
            if (m_grid[r][c] == Config::PieceType::None)
            {
                return false;
            }
        }
    }
    return true;
}
