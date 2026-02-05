#include "ResourceManager.h"
#include <QDir>          // 用于处理资源路径
#include <QStandardPaths> // 可选：处理跨平台资源路径
#include "../story/Constants.h"
#ifndef RESOURCE_ROOT
#define RESOURCE_ROOT "./res"
#endif

#include <QMediaPlayer>
#include <QSoundEffect>
#include <QUrl>


/**
 * @brief 私有构造函数实现：初始化音频组件与缓存容器
 * 实现步骤（组员需补充的逻辑）：
 * Step1：初始化BGM播放器
 * - m_bgmPlayer = new QMediaPlayer(this); // 绑定父对象自动释放
 * - 设置BGM默认播放模式：m_bgmPlayer->setLoops(QMediaPlayer::Infinite); // 无限循环
 * - 监听BGM播放错误信号（可选）：connect(m_bgmPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error){ qWarning() << "BGM play error:" << error; });
 *
 * Step2：初始化音效缓存容器
 * - m_soundCache.clear(); // 确保缓存容器为空（可选，默认构造已为空）
 *
 * Step3：配置资源路径（可选）
 * - 验证res目录是否存在，不存在则打印警告日志。
 *
 * @param parent 父对象指针（单例模式下默认传nullptr）
 */
 *
 *
*/
ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent)
    , m_bgmPlayer(nullptr) // 先初始化指针为nullptr，避免野指针
    , m_bgmPlayer(nullptr)
{
    // 空实现：组员需按上述步骤补充代码
}

/**
 * @brief 单例实例获取函数实现
 * 原理：静态局部变量`inst`在第一次调用时初始化，生命周期贯穿整个程序，确保全局唯一。
 * 注意：C++11及以上标准中，静态局部变量的初始化是线程安全的，无需额外加锁。
 * @return ResourceManager& 全局唯一的资源管理器实例引用
 */
ResourceManager& ResourceManager::instance()
 ResourceManager & ResourceManager::instance()
{
    static ResourceManager inst; // 静态局部实例（仅初始化一次）
    static ResourceManager inst;
    return inst;
}

/**
 * @brief 图片资源获取函数空实现
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：缓存命中校验
 * - 检查m_textureCache中是否存在key为`filename`的图片；
 * - 若存在，直接返回缓存中的QPixmap对象。
 *
 * Step2：构建资源绝对路径
 * - 拼接资源根目录与文件名：QString path = QString("%1/images/%2").arg(Constants::RESOURCE_ROOT).arg(filename);
 * - （可选）处理跨平台路径分隔符：path = QDir::toNativeSeparators(path);
 *
 * Step3：从磁盘加载图片
 * - 初始化QPixmap对象并加载路径：QPixmap pixmap(path);
 * - 检查加载是否成功：若pixmap.isNull()，打印警告日志（qWarning() << "Load texture failed:" << path;）并返回空QPixmap；
 * - 若加载成功，将图片存入缓存（m_textureCache.insert(filename, pixmap)）。
 *
 * Step4：返回图片对象
 * - 返回加载后的QPixmap（缓存中已存在或新加载的）。
 *
 * @param filename 图片相对路径（基于res/images/）
 * @return QPixmap 加载后的图片（失败返回空）
 */
QPixmap ResourceManager::getTexture(const QString& filename)
{
    if (m_textureCache.contains(filename)) {
        return m_textureCache[filename];
    }

    // Step2：拼接路径，处理跨平台分隔符
    QString path = QString("%1/images/%2").arg(RESOURCE_ROOT).arg(filename);
    path = QDir::toNativeSeparators(path);

    // Step3：加载图片，失败返回空
    QPixmap pixmap;
    if (!pixmap.load(path)) {
        qWarning() << "图片加载失败：" << path;
        return QPixmap();
    }
    // 加载成功，存入缓存
    m_textureCache.insert(filename,pixmap);

    // Step4：返回图片
    return pixmap;
    // 空实现：返回空图片，组员需按上述步骤补充代码
    return QPixmap();
}

/**
 * @brief 短音效播放函数空实现
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：缓存命中校验
 * - 检查m_soundCache中是否存在key为`filename`的音效对象；
 * - 若不存在，执行Step2创建并缓存；若存在，直接执行Step3。
 *
 * Step2：创建并缓存音效对象
 * - 构建音效绝对路径：QString path = QString("%1/audio/%2").arg(Constants::RESOURCE_ROOT).arg(filename);
 * - 新建QSoundEffect对象：QSoundEffect* sound = new QSoundEffect(this);
 * - 设置音效文件路径：sound->setSource(QUrl::fromLocalFile(path));
 * - （可选）设置默认音量：sound->setVolume(0.5); // 音量范围0.0-1.0
 * - 检查音效加载是否成功：若sound->status() != QSoundEffect::Ready，打印警告并删除对象，直接返回；
 * - 将音效对象存入缓存：m_soundCache.insert(filename, sound);
 *
 * Step3：播放音效
 * - 调用音效对象的play()方法：m_soundCache[filename]->play();
 *
 * @param filename 音效相对路径（基于res/audio/）
 */
void ResourceManager::playSound(const QString& filename)
{
    // 空实现：组员需按上述步骤补充代码
}
    if (m_soundCache.contains(filename))
    {
        QSoundEffect* sound = m_soundCache[filename];
        // 校验音效状态，避免异常播放
        if (sound->status() == QSoundEffect::Ready)
        {
            sound->play();
        }
        return;
    }

    // Step2：缓存未命中，构建音效绝对路径
    QString soundPath = QString("%1/audio/%2").arg(RESOURCE_ROOT).arg(filename);
    soundPath = QDir::toNativeSeparators(soundPath);

    // Step3：创建QSoundEffect对象，绑定父对象（当前单例）实现自动内存管理
    QSoundEffect* newSound = new QSoundEffect(this);
    // 设置音效源文件（QSoundEffect要求QUrl格式，本地文件用fromLocalFile）
    newSound->setSource(QUrl::fromLocalFile(soundPath));
    // 设置默认音量（0.0-1.0，符合游戏常规默认值）
    newSound->setVolume(0.5);

    // Step4：校验音效加载状态，失败则清理资源并打印日志
    if (newSound->status() != QSoundEffect::Ready)
    {
        qWarning() << "[ResourceManager] 音效加载失败：路径错误或格式不支持 -> " << soundPath;
        delete newSound; // 手动删除，避免内存泄漏
        return;
    }

    // Step5：加载成功，存入缓存并立即播放
    m_soundCache.insert(filename, newSound);
    qDebug() << "[ResourceManager] 音效加载并缓存成功 -> " << filename;
    newSound->play();
}
void ResourceManager::playBGM(const QString& filename)
{
    // 空实现：组员需按上述步骤补充代码
    // 懒加载初始化播放器+播放列表
    if (m_bgmPlayer == nullptr) {
        m_bgmPlayer = new QMediaPlayer(this);

        // Qt6 需要 QAudioOutput 来处理音频输出
        QAudioOutput* audioOutput = new QAudioOutput(this);
        m_bgmPlayer->setAudioOutput(audioOutput);

        // 设置默认音量
        audioOutput->setVolume(0.5);

        // Qt6 使用 setLoops 设置循环模式（替代了 setPlaybackMode）
        m_bgmPlayer->setLoops(QMediaPlayer::Infinite); // 无限循环

        // 错误监听
        connect(m_bgmPlayer, &QMediaPlayer::errorOccurred, this,
                [](QMediaPlayer::Error error, const QString& errorString) {
                    qWarning() << "[ResourceManager] BGM错误：" << error << " -> " << errorString;
                });
    }

    // 构建跨平台BGM路径
    QString bgmPath = QString("%1/audio/%2").arg(RESOURCE_ROOT).arg(filename);
    bgmPath = QDir::toNativeSeparators(bgmPath);

    // 停止当前播放的BGM
    if (m_bgmPlayer->playbackState() == QMediaPlayer::PlayingState) {
        m_bgmPlayer->stop();
    }

    // Qt6 直接使用 setSource 设置音频源（替代了 playlist 方式）
    m_bgmPlayer->setSource(QUrl::fromLocalFile(bgmPath));

    // 开始播放
    m_bgmPlayer->play();
    qDebug() << "[ResourceManager] BGM开始播放：" << filename;


}
