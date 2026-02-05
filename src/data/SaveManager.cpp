
#include "SaveManager.h"
#include <QFile>          // 用于文件读写操作
#include <QJsonDocument>  // 用于JSON对象与字符串的转换
#include <QDir>           // 用于目录创建与路径处理
#include <QStandardPaths> // 用于获取系统标准用户目录（跨平台关键）
#include <QDateTime>      // 用于生成存档时间戳（可选，建议补充）
#include "../story/Constants.h"






 /* @brief 无参构造函数实现（委托构造）
 * 说明：调用带参构造，将父对象设为nullptr，避免重复编写初始化代码，符合DRY（Don't Repeat Yourself）原则。
 */

SaveManager::SaveManager()
    : SaveManager(nullptr) // 委托给带参构造，父对象为nullptr
{

}

/**
 * @brief 带参构造函数实现：初始化存档目录
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：获取系统标准存档目录
 * - 调用QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)获取应用本地数据目录；
 * - 拼接游戏专属存档目录：QString saveDir = QString("%1/%2/saves").arg(localDataDir).arg(Constants::GAME_NAME);
 *
 * Step2：检查并创建存档目录
 * - 初始化QDir对象：QDir dir(saveDir);
 * - 若目录不存在，调用dir.mkpath(".")递归创建所有层级目录；
 * - 若创建失败，打印警告日志（qWarning() << "Create save directory failed:" << saveDir;）。
 *
 * Step3：初始化其他配置（可选）
 * - 如设置存档文件后缀名、默认编码格式等。
 *
 * @param parent 父对象指针（由AppController传入，管理生命周期）
 */
SaveManager::SaveManager(QObject *parent)
    : QObject(parent) // 调用父类QObject的构造函数
{
    QString localDataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (localDataDir.isEmpty()) {
        qWarning() << "SaveManager: Get system local data directory failed!";
        return;
    }

    // Step2：拼接游戏专属存档目录（GAME_NAME来自Constants.h）
    QString gameSaveDir = QString("%1/%2/saves").arg(localDataDir).arg(Config::GAME_NAME);

    // Step3：递归创建目录（mkpath会创建所有不存在的父层级）
    QDir dir(gameSaveDir);
    if (!dir.exists() && !dir.mkpath(".")) {
        qWarning() << "SaveManager: Create save directory failed! Path:" << gameSaveDir;
    } else {
        qInfo() << "SaveManager: Game save directory init success! Path:" << gameSaveDir;
    }// 空实现：组员需按上述步骤补充代码
}

/**
 * @brief 保存游戏进度函数空实现
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：参数合法性校验
 * - 若gameData为空（gameData.isEmpty()），打印警告并返回false；
 * - 若slotName为空，默认设为"autosave"（防御性编程）。
 *
 * Step2：获取存档文件路径
 * - 调用getSaveFilePath(slotName)获取绝对路径，记为filePath；
 * - 若filePath为空，返回false。
 *
 * Step3：准备存档数据（可选，补充时间戳）
 * - QJsonObject saveData = gameData; // 拷贝传入的游戏数据
 * - saveData["saveTime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); // 添加存档时间
 *
 * Step4：写入文件
 * - 初始化QFile对象：QFile file(filePath);
 * - 以只写+截断模式打开文件：if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) { 打印错误并返回false; }
 * - 将JSON对象转换为字符串：QJsonDocument doc(saveData); QByteArray jsonData = doc.toJson(QJsonDocument::Indented); // 格式化输出，便于调试
 * - 写入文件：qint64 bytesWritten = file.write(jsonData);
 * - 关闭文件：file.close();
 *
 * Step5：结果判断与日志输出
 * - 若bytesWritten == -1，打印文件写入错误日志并返回false；
 * - 否则打印成功日志（qInfo() << "Save game success:" << filePath;）并返回true。
 *
 * @param gameData 待保存的游戏数据JSON对象
 * @param slotName 存档槽位名称
 * @return bool 保存结果
 */
bool SaveManager::saveGame(const QJsonObject& gameData, const QString& slotName)
{
    // 空实现：返回false，组员需按上述步骤补充代码
    if (gameData.isEmpty()) {
        qWarning() << "SaveManager: Save game failed - gameData is empty!";
        return false;
    }
    // 防御性编程：槽位名为空则默认设为autosave
    QString actualSlot = slotName.isEmpty() ? "autosave" : slotName;

    // Step2：获取存档文件绝对路径，路径无效则直接返回
    QString filePath = getSaveFilePath(actualSlot);
    if (filePath.isEmpty()) {
        qWarning() << "SaveManager: Save game failed - invalid file path! Slot:" << actualSlot;
        return false;
    }

    // Step3：准备存档数据（拷贝原数据+补充存档时间戳，不修改入参）
    QJsonObject saveData = gameData;
    saveData["saveTime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // Step4：写入JSON数据到文件
    QFile file(filePath);
    // 只写+截断模式（覆盖原有文件，无则新建）
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "SaveManager: Open save file failed! Error:" << file.errorString() << "Path:" << filePath;
        return false;
    }
    // JSON格式化输出（Indented），便于调试和手动修改存档
    QJsonDocument doc(saveData);
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
    qint64 bytesWritten = file.write(jsonData);
    file.close(); // 及时关闭文件，释放资源

    // Step5：结果判断与日志输出
    if (bytesWritten == -1) {
        qCritical() << "SaveManager: Write save file failed! Path:" << filePath;
        return false;
    } else {
        qInfo() << "SaveManager: Save game success! Slot:" << actualSlot << "Path:" << filePath;
        return true;
    }
}

/**
 * @brief 读取游戏进度函数空实现
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：获取存档文件路径并检查文件存在性
 * - 调用getSaveFilePath(slotName)获取绝对路径filePath；
 * - 初始化QFile对象：QFile file(filePath);
 * - 若文件不存在或无法以只读模式打开（!file.open(QIODevice::ReadOnly)），打印警告并返回空QJsonObject；
 *
 * Step2：读取文件内容并解析JSON
 * - 读取文件所有内容：QByteArray jsonData = file.readAll();
 * - 关闭文件：file.close();
 * - 解析JSON数据：QJsonParseError parseError; QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
 * - 若解析失败（parseError.error != QJsonParseError::NoError），打印解析错误日志并返回空QJsonObject；
 * - 若解析结果不是JSON对象（!doc.isObject()），打印警告并返回空QJsonObject；
 *
 * Step3：返回解析后的存档数据
 * - 打印读取成功日志（qInfo() << "Load game success:" << filePath;）；
 * - 返回doc.object()。
 *
 * @param slotName 存档槽位名称
 * @return QJsonObject 读取到的存档数据（失败返回空）
 */
QJsonObject SaveManager::loadGame(const QString& slotName)
{
    QString actualSlot = slotName.isEmpty() ? "autosave" : slotName;
    QString filePath = getSaveFilePath(actualSlot);

    // Step1：检查文件存在性并尝试打开
    QFile file(filePath);
    if (!file.exists()) {
        qWarning() << "SaveManager: Load game failed - file not exist! Slot:" << actualSlot << "Path:" << filePath;
        return QJsonObject();
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "SaveManager: Open load file failed! Error:" << file.errorString() << "Path:" << filePath;
        return QJsonObject();
    }

    // Step2：读取文件内容并关闭
    QByteArray jsonData = file.readAll();
    file.close();

    // Step3：解析JSON数据，处理解析错误
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qCritical() << "SaveManager: JSON parse failed! Error:" << parseError.errorString()
        << "Offset:" << parseError.offset << "Path:" << filePath;
        return QJsonObject();
    }
    // 校验解析结果是否为JSON对象（避免存档文件是数组/其他格式）
    if (!doc.isObject()) {
        qWarning() << "SaveManager: Load game failed - not a JSON object! Path:" << filePath;
        return QJsonObject();
    }

    // Step4：读取成功，返回解析后的JSON对象
    qInfo() << "SaveManager: Load game success! Slot:" << actualSlot << "Path:" << filePath;
    return doc.object();
}

/**
 * @brief 检查存档是否存在函数空实现
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：获取存档文件路径
 * - 调用getSaveFilePath(slotName)获取绝对路径filePath；
 *
 * Step2：检查文件有效性
 * - 初始化QFileInfo对象：QFileInfo fileInfo(filePath);
 * - 检查文件是否存在且是普通文件（不是目录）：fileInfo.exists() && fileInfo.isFile();
 * - （可选）简单校验文件是否损坏：检查文件大小是否大于0，或读取前几个字节判断是否为JSON格式；
 *
 * Step3：返回检查结果
 * - 存在且有效返回true，否则返回false。
 *
 * @param slotName 存档槽位名称
 * @return bool 存档存在性结果
 */
bool SaveManager::hasSave(const QString& slotName)
{
    // 空实现：返回false，组员需按上述步骤补充代码
    if (slotName.isEmpty()) {
        qWarning() << "SaveManager: Check save failed - slotName is empty!";
        return false;
    }

    QString filePath = getSaveFilePath(slotName);
    QFileInfo fileInfo(filePath);

    // 严格校验：文件存在 + 是普通文件（非目录） + 文件大小大于0（避免空存档）
    bool isValid = fileInfo.exists() && fileInfo.isFile() && fileInfo.size() > 0;
    if (!isValid) {
        qDebug() << "SaveManager: Save slot not exist or invalid! Slot:" << slotName;
    }
    return isValid;
}

/**
 * @brief 生成存档文件路径私有函数空实现
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：获取系统标准用户目录
 * - QString localDataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
 * - 若localDataDir为空（获取失败），打印警告并返回空字符串；
 *
 * Step2：拼接游戏专属存档目录
 * - QString gameSaveDir = QString("%1/%2/saves").arg(localDataDir).arg(Constants::GAME_NAME); // GAME_NAME在Constants.h中定义，如"LQHJ20"
 *
 * Step3：确保目录存在
 * - QDir dir(gameSaveDir);
 * - 若dir.mkpath(".")失败，打印警告并返回空字符串；
 *
 * Step4：拼接存档文件全名
 * - QString fileName = QString("%1.save").arg(slotName); // 存档文件后缀为.save
 * - QString filePath = dir.filePath(fileName); // 生成跨平台的绝对路径
 *
 * Step5：返回路径
 * - 返回filePath。
 *
 * @param slotName 存档槽位名称
 * @return QString 存档文件绝对路径（失败返回空）
 */
QString SaveManager::getSaveFilePath(const QString& slotName) const
{
    // 空实现：返回空字符串，组员需按上述步骤补充代码
    QString localDataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (localDataDir.isEmpty()) {
        qWarning() << "SaveManager: Get local data dir failed when generate file path!";
        return "";
    }

    // Step2：拼接游戏专属存档目录
    QString gameSaveDir = QString("%1/%2/saves").arg(localDataDir).arg(Config::GAME_NAME);

    // Step3：确保目录存在（双重保障，避免构造时创建失败的情况）
    QDir dir(gameSaveDir);
    if (!dir.mkpath(".")) {
        qWarning() << "SaveManager: Ensure save dir failed! Path:" << gameSaveDir;
        return "";
    }

    // Step4：拼接存档文件名（后缀.save，便于识别），生成跨平台绝对路径
    QString fileName = QString("%1.save").arg(slotName);
    QString filePath = dir.filePath(fileName); // Qt自动处理跨平台路径分隔符（/ \）

    return filePath;
}
