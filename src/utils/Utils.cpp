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
    // Step1：边界预处理，负数坐标直接返回无效
    if (mouseX < 0 || mouseY < 0)
    {
        return QPoint(-1, -1);
    }

    // Step2：计算棋盘居中偏移量（适配QML通用800*800 GameView）
    int boardTotalPix = Config::BOARD_SIZE * Config::CELL_SIZE;
    int offsetX = (800 - boardTotalPix) / 2;
    int offsetY = (800 - boardTotalPix) / 2;

    // Step3：减去偏移量计算网格行列索引（整数除法自动取整，适配鼠标点击）
    int col = (mouseX - offsetX) / Config::CELL_SIZE;
    int row = (mouseY - offsetY) / Config::CELL_SIZE;

    // Step4：网格索引边界校验，确保在0~14范围内
    if (row < 0 || row >= Config::BOARD_SIZE || col < 0 || col >= Config::BOARD_SIZE)
    {
        return QPoint(-1, -1);
    }

    // Step5：返回有效网格坐标（x=列col，y=行row，和棋盘索引对应）
    return QPoint(col, row);
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
    // Step1：网格索引边界校验，无效索引直接返回无效坐标
    if (row < 0 || row >= Config::BOARD_SIZE || col < 0 || col >= Config::BOARD_SIZE)
    {
        return QPoint(-1, -1);
    }

    // Step2：计算棋盘居中偏移量（和pixelToGrid保持一致，避免坐标错位）
    int boardTotalPix = Config::BOARD_SIZE * Config::CELL_SIZE;
    int offsetX = (800 - boardTotalPix) / 2;
    int offsetY = (800 - boardTotalPix) / 2;

    // Step3：计算棋子中心点像素坐标（+CELL_SIZE/2 让棋子居中在格子里，UI更美观）
    int pixelX = col * Config::CELL_SIZE + Config::CELL_SIZE / 2 + offsetX;
    int pixelY = row * Config::CELL_SIZE + Config::CELL_SIZE / 2 + offsetY;

    // Step4：返回有效中心点像素坐标（QML可直接用该坐标设置棋子x/y）
    return QPoint(pixelX, pixelY);
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
    // Step1：获取当前系统时间并格式化为 年-月-日 时:分:秒 格式
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeStr = currentTime.toString("yyyy-MM-dd hh:mm:ss");

    // Step2：按固定格式拼接日志内容
    QString logStr = QString("[%1] [%2] [%3] %4")
                         .arg(timeStr)
                         .arg(module)
                         .arg(level)
                         .arg(message);

    // Step3：按日志级别输出
    if (level == "ERROR")
    {
        qCritical() << logStr;
    }
    else if (level == "WARN")
    {
        qWarning() << logStr;
    }
    else
    {
        // 非ERROR/WARN都按INFO级别输出，提高容错性
        qInfo() << logStr;
    }
}
