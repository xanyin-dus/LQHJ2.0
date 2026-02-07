#include "StoryManager.h"
#include <QDebug>        // 调试日志打印
#include <QJsonArray>    // JSON数组解析所需头文件
#include <QJsonObject>   // JSON对象解析所需头文件

/**
 * @brief 构造函数实现：初始化剧情管理状态
 * 详细实现逻辑：
 * 1. 调用父类QObject构造函数，绑定父对象；
 * 2. 初始化当前剧情帧ID为空字符串；
 * 3. 初始化当前剧情帧为默认空值（通过StoryFrame的默认构造）；
 * 4. 清空剧情章节数据缓存（m_currentChapterData.clear()）；
 * 5. 打印初始化日志，便于调试。
 * @param parent 父对象指针（由AppController传入）
 */
StoryManager::StoryManager(QObject *parent)
    : QObject(parent)
    , m_currentFrameId("")  // 初始化当前帧ID为空
    , m_currentFrame()      // 初始化空剧情帧
{
    m_currentChapterData.clear(); // 清空缓存
    qInfo() << "[StoryManager] 初始化完成";
}

/**
 * @brief 加载剧情章节函数空实现
 * 详细实现步骤（组员需补充核心逻辑）：
 * Step1：拼接剧情文件路径
 * - 基于Constants.h的资源路径，拼接qrc路径：QString filePath = QString("%1story/%2").arg(Config::IMG_PATH).arg(jsonFileName);
 * - 修正：剧情文件路径建议在Constants.h新增`const QString STORY_PATH = "qrc:/res/story/";`，拼接为`STORY_PATH + jsonFileName`。
 * Step2：校验文件存在性
 * - 初始化QFile file(filePath); 若!file.exists()，打印警告并返回。
 * Step3：解析JSON文件
 * - 调用parseJson(filePath)，将解析后的数据存入m_currentChapterData。
 * Step4：初始化当前剧情帧
 * - 从JSON中读取初始帧ID（如根节点的"startFrame"字段），记为startId；
 * - 校验startId是否存在于m_currentChapterData中，不存在则打印警告并返回；
 * - 更新m_currentFrameId = startId，m_currentFrame = m_currentChapterData[startId]。
 * Step5：同步UI与播放BGM
 * - 发射frameUpdate()信号，通知QML刷新；
 * - 若当前帧的bgm不为空，调用ResourceManager::instance().playBGM(m_currentFrame.bgm)。
 * @param jsonFileName 剧情JSON文件名（如"prologue.json"）
 */
void StoryManager::loadChapter(const QString& jsonFileName)
{
    // 空实现：组员需按上述步骤补充代码
    Q_UNUSED(jsonFileName); // 消除未使用参数警告
    qInfo() << "[StoryManager] 加载剧情章节：" << jsonFileName;
}

/**
 * @brief 下一页剧情函数空实现
 * 详细实现步骤（组员需补充核心逻辑）：
 * Step1：剧情结束校验
 * - 若m_currentFrame.isFinalFrame（需在StoryFrame中新增该字段），发射chapterFinished()并返回。
 * Step2：获取下一个帧ID
 * - 方案1（顺序剧情）：解析当前帧ID的序号，+1得到下一个帧ID（如frame_001→frame_002）；
 * - 方案2（默认跳转）：在StoryFrame中新增`jumpToID`字段，无选项时跳转到该ID；
 * - 优先使用方案2（更灵活），若未设置则使用方案1。
 * Step3：校验下一个帧ID的有效性
 * - 若m_currentChapterData.contains(nextFrameId)为false，打印警告并返回。
 * Step4：更新当前帧并同步UI
 * - m_currentFrameId = nextFrameId; m_currentFrame = m_currentChapterData[nextFrameId];
 * - 发射frameUpdate()信号；
 * - 若当前帧bgm变更，调用ResourceManager切换BGM。
 */
void StoryManager::next()
{
    // 空实现：组员需按上述步骤补充代码
    qInfo() << "[StoryManager] 进入下一页剧情";
}

/**
 * @brief 选择剧情选项函数空实现
 * 详细实现步骤（组员需补充核心逻辑）：
 * Step1：选项索引合法性校验
 * - 若optionIndex < 0 或 optionIndex >= m_currentFrame.options.size()，打印警告并返回。
 * Step2：获取目标帧ID
 * - StoryOption selectedOption = m_currentFrame.options[optionIndex];
 * - QString targetFrameId = selectedOption.jumpToID;
 * Step3：目标帧有效性校验
 * - 若!m_currentChapterData.contains(targetFrameId)，打印警告并返回。
 * Step4：更新当前帧并同步UI
 * - m_currentFrameId = targetFrameId; m_currentFrame = m_currentChapterData[targetFrameId];
 * - 发射frameUpdate()信号；
 * - 播放选项选择音效：ResourceManager::instance().playSound("select_option.wav");
 * - 若目标帧bgm变更，切换BGM。
 * @param optionIndex 选项索引（从0开始）
 */
void StoryManager::chooseOption(int optionIndex)
{
    // 空实现：组员需按上述步骤补充代码
    Q_UNUSED(optionIndex); // 消除未使用参数警告
    qInfo() << "[StoryManager] 选择剧情选项，索引：" << optionIndex;
}

/**
 * @brief 解析JSON剧情文件私有函数空实现
 * 详细实现步骤（组员需补充核心逻辑）：
 * Step1：打开并读取JSON文件
 * - QFile file(path); 若!file.open(QIODevice::ReadOnly)，打印错误日志并返回；
 * - QByteArray jsonData = file.readAll(); file.close();
 * Step2：解析JSON数据
 * - QJsonParseError parseError;
 * - QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
 * - 若parseError.error != QJsonParseError::NoError，打印解析错误并返回；
 * - 若!doc.isObject()，打印警告并返回；
 * - QJsonObject rootObj = doc.object();
 * Step3：遍历剧情帧数组
 * - QJsonArray framesArray = rootObj["frames"].toArray();
 * - 遍历framesArray的每个元素：
 *   1. QJsonObject frameObj = item.toObject();
 *   2. 构造StoryFrame对象，赋值各字段：
 *      - id = frameObj["id"].toString();
 *      - speaker = frameObj["speaker"].toString();
 *      - text = frameObj["text"].toString();
 *      - bgImage = frameObj["bgImage"].toString();
 *      - bgm = frameObj["bgm"].toString();
 *      - isFinalFrame = frameObj["isFinalFrame"].toBool(false);
 *   3. 解析选项列表：
 *      - QJsonArray optionsArray = frameObj["options"].toArray();
 *      - 遍历optionsArray，构造StoryOption对象（text=...，jumpToID=...），加入frame.options；
 *   4. 将frame存入m_currentChapterData：m_currentChapterData.insert(frame.id, frame);
 * Step4：打印解析结果日志
 * - qInfo() << "[StoryManager] 解析完成，加载" << framesArray.size() << "个剧情帧";
 * @param path JSON文件的绝对路径
 */
void StoryManager::parseJson(const QString& path)
{
    // 空实现：组员需按上述步骤补充代码
    Q_UNUSED(path); // 消除未使用参数警告
    qInfo() << "[StoryManager] 解析JSON文件：" << path;
}

/**
 * @brief 获取当前剧情文本的READ函数实现
 * 详细实现逻辑：
 * 直接返回m_currentFrame.text，若为空则返回空字符串（防御性编程）。
 * @return QString 当前剧情文本
 */
QString StoryManager::text() const
{
    // 空实现：补充为返回m_currentFrame.text
    return m_currentFrame.text; // 修正：原返回空字符串，改为返回当前帧文本
}

/**
 * @brief 获取当前说话人的READ函数实现
 * 详细实现逻辑：
 * 直接返回m_currentFrame.speaker，若为空则返回空字符串。
 * @return QString 当前说话人名称
 */
QString StoryManager::speaker() const
{
    // 空实现：补充为返回m_currentFrame.speaker
    return m_currentFrame.speaker; // 修正：原返回空字符串，改为返回当前帧说话人
}

/**
 * @brief 获取当前背景图路径的READ函数实现
 * 详细实现逻辑：
 * 直接返回m_currentFrame.bgImage，若为空则返回默认背景图路径（可选扩展）。
 * @return QString 当前背景图路径
 */
QString StoryManager::bgImage() const
{
    // 空实现：补充为返回m_currentFrame.bgImage
    return m_currentFrame.bgImage; // 修正：原返回空字符串，改为返回当前帧背景图
}

/**
 * @brief 获取选项文本列表的READ函数实现（新增）
 * 详细实现逻辑：
 * 遍历m_currentFrame.options，提取每个选项的text字段，存入QList<QString>并返回。
 * @return QList<QString> 选项文本列表
 */
QList<QString> StoryManager::optionTexts() const
{
    // 空实现：组员需补充遍历逻辑
    QList<QString> texts;
    for (const auto& option : m_currentFrame.options) {
        texts.append(option.text);
    }
    return texts;
}
