#pragma once
#ifndef STORYMANAGER_H
#define STORYMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QJsonDocument>  // JSON解析所需头文件
#include <QFile>          // 文件读取所需头文件
// 统一引用剧情数据结构，删除本地重复定义的StoryFrame
#include "StoryChapter.h"
// 引入资源管理器，用于加载剧情背景图/BGM
#include "../data/ResourceManager.h"
// 引入全局常量，规范剧情文件路径
#include "../story/Constants.h"

/**
 * @brief 剧情管理核心控制器
 * 核心职责：
 * 1. 加载并解析res/story/目录下的JSON剧情配置文件，转换为StoryFrame数据结构；
 * 2. 管理剧情帧的跳转逻辑（点击继续→下一页、选择选项→分支跳转）；
 * 3. 通过Qt属性系统将当前剧情帧数据（文本、说话人、背景图）同步给QML；
 * 4. 处理剧情章节的开始与结束，触发界面切换信号（如剧情结束跳转到游戏界面）；
 * 5. 与SaveManager联动，保存/读取当前剧情帧ID（存档功能）。
 * 设计特点：继承QObject支持信号槽，QML可直接调用核心接口，采用QMap缓存剧情帧（通过ID快速定位）。
 */
class StoryManager : public QObject
{
    Q_OBJECT  // Qt信号槽/属性系统必须的宏

    // 暴露给QML的属性：QML直接绑定，属性更新时自动发射frameUpdate信号刷新UI
    /**
     * @brief 当前剧情帧的对话文本
     * QML绑定场景：StoryView中显示剧情内容的文本框。
     */
    Q_PROPERTY(QString text READ text NOTIFY frameUpdate)
    /**
     * @brief 当前剧情帧的说话人名称
     * QML绑定场景：StoryView中显示角色名称的标签。
     */
    Q_PROPERTY(QString speaker READ speaker NOTIFY frameUpdate)
    /**
     * @brief 当前剧情帧的背景图路径
     * QML绑定场景：StoryView中作为背景的Image组件。
     */
    Q_PROPERTY(QString bgImage READ bgImage NOTIFY frameUpdate)
    /**
     * @brief 当前剧情帧的分支选项列表（新增）
     * QML绑定场景：StoryView中动态生成选项按钮，无选项则隐藏。
     */
    Q_PROPERTY(QList<QString> optionTexts READ optionTexts NOTIFY frameUpdate)

public:
    /**
     * @brief 构造函数（显式构造，禁止隐式转换）
     * @param parent 父对象指针（由AppController管理生命周期）
     * 初始化逻辑：
     * 1. 初始化当前剧情帧ID和空帧数据；
     * 2. 清空剧情章节数据缓存；
     * 3. （可选）预加载常用剧情资源。
     */
    explicit StoryManager(QObject *parent = nullptr);

    /**
     * @brief 加载指定剧情章节（QML可调用）
     * @param jsonFileName 剧情JSON文件名（基于res/story/目录，如"prologue.json"）
     * 功能逻辑：
     * 1. 拼接剧情文件的绝对路径（qrc:/res/story/ + jsonFileName）；
     * 2. 调用parseJson()解析JSON文件，将数据存入m_currentChapterData；
     * 3. 初始化当前剧情帧为章节的第一个帧（需在JSON中标记初始帧ID，如"start"）；
     * 4. 触发frameUpdate信号，同步数据到QML；
     * 5. （可选）播放章节初始BGM（调用ResourceManager）。
     */
    Q_INVOKABLE void loadChapter(const QString& jsonFileName);

    /**
     * @brief 进入下一个剧情帧（QML可调用）
     * 适用场景：当前剧情帧无分支选项时，玩家点击“继续”按钮触发。
     * 功能逻辑：
     * 1. 校验当前剧情帧是否为最终帧（isFinalFrame），若是则发射chapterFinished信号；
     * 2. 若当前帧有默认跳转ID（可选扩展），跳转到对应帧；
     * 3. 若为顺序剧情，按帧ID规则（如frame_001→frame_002）跳转到下一帧；
     * 4. 更新当前帧数据，触发frameUpdate信号。
     */
    Q_INVOKABLE void next();

    /**
     * @brief 选择剧情分支选项（QML可调用）
     * @param optionIndex 选项索引（从0开始，对应optionTexts的下标）
     * 适用场景：当前剧情帧有分支选项时，玩家点击某选项触发。
     * 功能逻辑：
     * 1. 校验optionIndex是否合法（0 ≤ index < options.size()）；
     * 2. 获取选中选项的jumpToID（目标剧情帧ID）；
     * 3. 校验jumpToID对应的剧情帧是否存在；
     * 4. 更新当前帧为目标帧，触发frameUpdate信号；
     * 5. （可选）播放选项选择音效（调用ResourceManager）。
     */
    Q_INVOKABLE void chooseOption(int optionIndex);

    // Q_PROPERTY对应的READ函数（const修饰，仅查询不修改）
    /**
     * @brief 获取当前剧情帧的对话文本
     * @return QString 对话文本（为空则返回空字符串）
     */
    QString text() const;

    /**
     * @brief 获取当前剧情帧的说话人名称
     * @return QString 说话人名称（为空则返回空字符串）
     */
    QString speaker() const;

    /**
     * @brief 获取当前剧情帧的背景图路径
     * @return QString 背景图路径（为空则返回空字符串）
     */
    QString bgImage() const;

    /**
     * @brief 获取当前剧情帧的选项文本列表（新增）
     * @return QList<QString> 选项文本列表（无选项则返回空列表）
     * 作用：QML根据此列表动态生成选项按钮。
     */
    QList<QString> optionTexts() const;

signals:
    /**
     * @brief 剧情帧更新信号（通知QML刷新UI）
     * 触发时机：加载新章节、调用next()、选择选项后，当前帧数据变更时。
     */
    void frameUpdate();

    /**
     * @brief 剧情章节结束信号（通知AppController切换界面）
     * 触发时机：当前剧情帧为最终帧（isFinalFrame=true）时。
     * AppController响应逻辑：跳转到GameView或MainMenuView。
     */
    void chapterFinished();

private:
    /**
     * @brief 私有辅助函数：解析JSON剧情文件
     * @param path 剧情文件的绝对路径（qrc路径或本地路径）
     * 核心逻辑：
     * 1. 打开JSON文件并读取所有内容；
     * 2. 解析JSON为QJsonDocument，转换为QJsonObject；
     * 3. 遍历JSON中的"frames"数组，将每个元素转换为StoryFrame结构体；
     * 4. 将StoryFrame存入m_currentChapterData（key=帧ID，value=帧数据）；
     * 5. 处理解析错误（文件不存在、JSON格式错误），打印警告日志。
     */
    void parseJson(const QString& path);

    // 私有成员变量
    /**
     * @brief 当前章节的剧情帧缓存（帧ID→帧数据）
     * 作用：通过帧ID快速定位目标剧情帧，支持分支跳转和存档定位。
     */
    QMap<QString, StoryFrame> m_currentChapterData;

    /**
     * @brief 当前剧情帧的唯一ID
     * 作用：标记当前进度，用于存档和帧跳转。
     */
    QString m_currentFrameId;

    /**
     * @brief 当前剧情帧的完整数据
     * 作用：存储当前帧的文本、说话人、背景图、选项等数据，供QML读取。
     */
    StoryFrame m_currentFrame;
};

#endif // STORYMANAGER_H
