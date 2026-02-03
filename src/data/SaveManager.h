#pragma once
#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QString>
#include <QDebug>

/**
 * @brief 游戏存档管理类
 * 核心职责：
 * 1. 统一处理游戏存档的**保存（序列化）**与**读取（反序列化）**，采用JSON格式存储存档数据；
 * 2. 支持多存档槽位（自动存档autosave、手动存档slot1/slot2等）；
 * 3. 管理跨平台的存档文件路径（基于Qt标准用户目录，避免权限问题）；
 * 4. 提供QML可调用的存档存在性检查接口，支持主菜单/加载界面的存档状态判断；
 * 5. 处理存档操作的异常（文件写入失败、JSON解析错误等），返回明确的操作结果。
 * 数据交互：与GameController（获取棋盘/玩家数据）、StoryManager（获取剧情节点）、AppController（暴露给QML）交互。
 */
class SaveManager : public QObject {
    Q_OBJECT  // 支持信号槽，后续可扩展存档成功/失败信号

public:
    /**
     * @brief 无参构造函数（委托给带参构造）
     * 说明：简化外部调用，默认父对象为nullptr，内部调用带参构造完成初始化。
     */
    SaveManager();

    /**
     * @brief 带参构造函数（显式构造，禁止隐式转换）
     * @param parent 父对象指针（遵循Qt父子内存管理，由AppController管理生命周期）
     * 初始化逻辑：
     * 1. 获取系统标准存档目录（如Windows的AppData、Linux的~/.config）；
     * 2. 检查游戏专属存档目录是否存在，不存在则递归创建；
     * 3. 初始化存档文件的后缀名（如".save"）等全局配置。
     */
    explicit SaveManager(QObject *parent = nullptr);

    /**
     * @brief 保存游戏进度到指定存档槽位
     * @param gameData 待保存的游戏数据JSON对象（必须包含的关键字段见下方说明）
     * @param slotName 存档槽位名称（默认值"autosave"为自动存档，手动存档可传"slot1"/"slot2"等）
     * @return bool 保存结果：true=保存成功，false=保存失败（文件写入失败/路径无效/JSON为空）
     * gameData 必须包含的核心字段：
     * - "boardState": 棋盘状态（二维数组，如[[0,1,0],[2,1,0],...]，0=空，1=黑棋，2=白棋）；
     * - "currentChapter": 当前剧情章节ID（字符串，如"story_chapter_1"）；
     * - "playerInfo": 玩家信息（JSON对象，包含"name"/"chessColor"/"isAI"）；
     * - "settings": 游戏设置（JSON对象，包含"bgmVolume"/"soundVolume"/"boardSize"）；
     * - "saveTime": 存档时间（字符串，格式如"2026-02-16 20:30:00"）。
     */
    bool saveGame(const QJsonObject& gameData, const QString& slotName = "autosave");

    /**
     * @brief 从指定存档槽位读取游戏进度
     * @param slotName 存档槽位名称（默认读取自动存档"autosave"）
     * @return QJsonObject 读取到的存档数据：
     * - 读取成功：返回包含完整游戏数据的JSON对象（字段同saveGame的gameData）；
     * - 读取失败：返回空QJsonObject（文件不存在/文件损坏/JSON解析错误）。
     * 适用场景：GameController加载棋盘状态、StoryManager加载剧情节点、SettingsView加载设置参数。
     */
    QJsonObject loadGame(const QString& slotName = "autosave");

    /**
     * @brief 检查指定存档槽位是否存在有效存档
     * @param slotName 存档槽位名称（如"autosave"/"slot1"）
     * @return bool 检查结果：true=存在有效存档，false=不存在或存档损坏
     * @note Q_INVOKABLE标记：让QML可直接调用（如主菜单的"继续游戏"按钮是否可用）。
     */
    Q_INVOKABLE bool hasSave(const QString& slotName);

private:
    /**
     * @brief 私有辅助函数：生成指定存档槽位的绝对文件路径
     * @param slotName 存档槽位名称
     * @return QString 存档文件的绝对路径（如"C:/Users/xxx/AppData/Local/LQHJ20/saves/autosave.save"）
     * 实现逻辑：
     * 1. 拼接系统标准用户目录 + 游戏专属目录 + 存档文件名；
     * 2. 处理跨平台路径分隔符（Qt自动处理，无需手动转换）；
     * 3. 确保路径中的目录层级已存在（若不存在则创建）。
     */
    QString getSaveFilePath(const QString& slotName) const;
};

#endif // SAVEMANAGER_H
