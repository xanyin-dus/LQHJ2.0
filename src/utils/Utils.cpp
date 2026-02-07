#include "Utils.h"

/**
 * @brief 像素坐标转网格索引函数空实现
 * 详细实现步骤（组员需补充核心逻辑）：
 * Step1：边界预处理（可选）
 * - 若mouseX < 0 或 mouseY < 0，直接返回QPoint(-1,-1)（无效坐标）。
 * Step2：计算网格索引
 * - 核心公式：col = mouseX / Config::CELL_SIZE; row = mouseY / Config::CELL_SIZE;
 * - （进阶）若棋盘在GameView中居中显示，需先减去偏移量：
 *   int offsetX = (GameView宽度 - Config::BOARD_SIZE * Config::CELL_SIZE) / 2;
 *   int offsetY = (GameView高度 - Config::BOARD_SIZE * Config::CELL_SIZE) / 2;
 *   col = (mouseX - offsetX) / Config::CELL_SIZE;
 *   row = (mouseY - offsetY) / Config::CELL_SIZE;
 * Step3：网格索引边界校验
 * - 若row < 0 或 row >= Config::BOARD_SIZE 或 col < 0 或 col >= Config::BOARD_SIZE，返回QPoint(-1,-1)；
 * - 否则返回QPoint(col, row)（注意：QPoint的x对应列col，y对应行row）。
 * @param mouseX 鼠标X像素坐标
 * @param mouseY 鼠标Y像素坐标
 * @return QPoint 网格坐标（x=col, y=row），无效则返回(-1,-1)
 */
QPoint Utils::pixelToGrid(int mouseX, int mouseY)
{
    // 空实现：返回无效坐标，组员需按上述步骤补充代码
    Q_UNUSED(mouseX);
    Q_UNUSED(mouseY);
    return QPoint(-1, -1);
}

/**
 * @brief 网格索引转像素坐标函数空实现
 * 详细实现步骤（组员需补充核心逻辑）：
 * Step1：网格索引边界校验
 * - 若row < 0 或 row >= Config::BOARD_SIZE 或 col < 0 或 col >= Config::BOARD_SIZE，返回QPoint(-1,-1)。
 * Step2：计算中心点像素坐标
 * - 核心公式：x = col * Config::CELL_SIZE + Config::CELL_SIZE / 2;
 *            y = row * Config::CELL_SIZE + Config::CELL_SIZE / 2;
 * - （进阶）若棋盘在GameView中居中显示，需加上偏移量：
 *   int offsetX = (GameView宽度 - Config::BOARD_SIZE * Config::CELL_SIZE) / 2;
 *   int offsetY = (GameView高度 - Config::BOARD_SIZE * Config::CELL_SIZE) / 2;
 *   x += offsetX; y += offsetY;
 * Step3：返回结果
 * - 返回计算后的QPoint(x, y)（棋子显示的中心点坐标）。
 * @param row 棋盘行索引
 * @param col 棋盘列索引
 * @return QPoint 像素坐标，无效则返回(-1,-1)
 */
QPoint Utils::gridToPixel(int row, int col)
{
    // 空实现：返回无效坐标，组员需按上述步骤补充代码
    Q_UNUSED(row);
    Q_UNUSED(col);
    return QPoint(-1, -1);
}

/**
 * @brief 基础版日志函数实现（默认INFO级别）
 * 详细实现逻辑：
 * 调用带级别的重载log函数，传入默认级别"INFO"，减少代码冗余。
 * @param module 模块名
 * @param message 日志内容
 */
void Utils::log(const QString& module, const QString& message)
{
    // 调用重载函数，默认INFO级别
    Utils::log(module, message, "INFO");
}

/**
 * @brief 带级别的日志函数空实现（核心）
 * 详细实现步骤（组员需补充核心逻辑）：
 * Step1：格式化时间戳
 * - 获取当前系统时间：QDateTime currentTime = QDateTime::currentDateTime();
 * - 转换为字符串：QString timeStr = currentTime.toString("yyyy-MM-dd hh:mm:ss");
 * Step2：格式化日志内容
 * - 按固定格式拼接：QString logStr = QString("[%1] [%2] [%3] %4")
 *                                      .arg(timeStr)
 *                                      .arg(module)
 *                                      .arg(level)
 *                                      .arg(message);
 * Step3：按级别输出日志
 * - 若level == "ERROR"：qCritical() << logStr;（红色错误日志）
 * - 若level == "WARN"：qWarning() << logStr;（黄色警告日志）
 * - 若level == "INFO"：qInfo() << logStr;（蓝色信息日志）
 * Step4：（可选）日志写入文件
 * - 若需要持久化日志，可将logStr写入本地日志文件（如"log.txt"）。
 * @param module 模块名
 * @param message 日志内容
 * @param level 日志级别（INFO/ERROR/WARN）
 */
void Utils::log(const QString& module, const QString& message, const QString& level)
{
    // 空实现：组员需按上述步骤补充代码
    Q_UNUSED(module);
    Q_UNUSED(message);
    Q_UNUSED(level);
    // 示例：qInfo() << QString("[%1] [%2] [%3] %4").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(module).arg(level).arg(message);
}
