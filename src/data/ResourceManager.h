#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QMap>
#include <QMediaPlayer>
#include <QPixmap>
#include <QSoundEffect>
#include <QString>
#include <QDebug>

/**
 * @brief 全局资源管理单例类
 * 核心职责：
 * 1. 统一管理游戏所有静态资源（图片、音效、背景音乐）的加载与缓存；
 * 2. 提供高效的资源获取接口（优先从缓存读取，避免重复IO）；
 * 3. 区分短音效（QSoundEffect）和长音频（QMediaPlayer）的播放逻辑；
 * 4. 处理资源加载失败的异常（打印日志、返回默认资源）。
 * 设计模式：饿汉式单例（静态局部变量），线程安全（C++11后静态局部变量初始化线程安全）；
 * 资源路径约定：所有资源路径基于项目根目录的`res/`文件夹，调用时传入相对路径（如"images/board.png"）。
 */
class ResourceManager : public QObject {
    Q_OBJECT  // 支持信号槽，后续可扩展资源加载完成/失败信号

public:
    /**
     * @brief 获取全局唯一实例（单例核心接口）
     * @return ResourceManager& 全局资源管理器实例引用
     * 调用方式：ResourceManager::instance().getTexture("xxx.png") / playSound("xxx.wav")
     * 注意：禁止通过构造函数创建实例，仅能通过此接口访问。
     */
    static ResourceManager& instance();

    /**
     * @brief 获取图片资源（优先从缓存读取，缓存未命中则从磁盘加载）
     * @param filename 图片相对路径（基于res/images/，如"chess_black.png"）
     * @return QPixmap 加载后的图片对象（加载失败返回空QPixmap）
     * 适用场景：棋盘纹理、棋子图片、剧情背景图、UI按钮图片的加载。
     */
    QPixmap getTexture(const QString& filename);

    /**
     * @brief 播放短音效（同步播放，低延迟，适合点击、落子等瞬时音效）
     * @param filename 音效相对路径（基于res/audio/，如"drop_chess.wav"）
     * @note 用QSoundEffect实现，自动缓存音效对象避免重复创建；Q_INVOKABLE标记后QML可直接调用。
     * 适用场景：落子声、菜单点击声、剧情选项确认声。
     */
    Q_INVOKABLE void playSound(const QString& filename);

    /**
     * @brief 播放/切换背景音乐（异步播放，支持循环，适合剧情BGM、游戏背景乐）
     * @param filename BGM相对路径（基于res/audio/，如"story_bg.mp3"）
     * @note 用QMediaPlayer实现，自动停止上一首BGM并切换新曲目；支持MP3/WAV等格式；Q_INVOKABLE标记后QML可直接调用。
     * 适用场景：主菜单BGM、游戏对局BGM、剧情章节BGM切换。
     */
    Q_INVOKABLE void playBGM(const QString& filename);

private:
    /**
     * @brief 私有构造函数（单例模式：禁止外部实例化）
     * @param parent 父对象指针（设为nullptr，单例无需父对象管理）
     * 初始化逻辑：
     * 1. 初始化BGM播放器指针为nullptr；
     * 2. 配置默认的音频播放参数（如BGM循环播放、音效音量默认值）。
     */
    explicit ResourceManager(QObject *parent = nullptr);

    /**
     * @brief 图片资源缓存容器
     * Key：图片相对路径（如"chess_white.png"）；Value：加载后的QPixmap对象
     * 作用：缓存已加载的图片，避免重复从磁盘读取，提升UI渲染性能。
     */
    QMap<QString, QPixmap> m_textureCache;

    /**
     * @brief BGM播放器（负责长音频的播放、暂停、切换）
     * 说明：QMediaPlayer适合播放大体积、长时长的音频（如MP3），支持网络流和本地文件。
     */
    QMediaPlayer* m_bgmPlayer;

    /**
     * @brief 音效资源缓存容器
     * Key：音效相对路径（如"click_btn.wav"）；Value：QSoundEffect音效对象指针
     * 作用：缓存短音效对象，避免每次播放都创建新实例，降低CPU占用和延迟。
     */
    QMap<QString, QSoundEffect*> m_soundCache;

    // 禁用拷贝构造和赋值运算符（单例模式必须）
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};

#endif // RESOURCEMANAGER_H
