#pragma once
#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QString>
#include <QDebug>  // 用于打印全局导航/模块初始化日志

// 项目内子模块头文件（对应game/story/data模块的核心类）
#include "game/GameController.h"   // 游戏逻辑控制器（棋盘、回合、胜负）
#include "story/StoryManager.h"   // 剧情管理控制器（加载、分支、触发）
#include "data/SaveManager.h"     // 存档管理控制器（进度保存/读取）

/**
 * @brief 全局应用控制器类
 * 核心职责：
 * 1. 作为QML与C++的通信桥梁，暴露子模块接口给QML调用；
 * 2. 管理游戏内所有核心子模块（Game/Story/Save）的生命周期；
 * 3. 处理全局界面导航逻辑（主菜单→游戏→剧情→设置的切换）；
 * 4. 转发全局状态变化信号（如界面切换、模块初始化完成）。
 * 设计模式：单例模式（建议后续扩展，确保全局唯一实例），继承QObject支持Qt信号槽机制。
 */
class AppController : public QObject
{
    Q_OBJECT  // Qt信号槽必须的宏，不可删除

    // 暴露给QML的属性：QML可直接通过`app.game`/`app.story`调用子模块的方法/信号
    // READ：指定QML读取属性的函数；CONSTANT：表示属性值（指针）在实例生命周期内不变化
    /**
     * @brief 暴露给QML的游戏逻辑控制器属性
     * QML使用场景：GameView中调用落子、重置游戏等逻辑，监听游戏胜负信号。
     */
    Q_PROPERTY(GameController* game READ game CONSTANT)
    /**
     * @brief 暴露给QML的剧情管理控制器属性
     * QML使用场景：StoryView中加载剧情章节、选择剧情分支，监听剧情文本更新信号。
     */
    Q_PROPERTY(StoryManager* story READ story CONSTANT)
    /**
     * @brief 暴露给QML的存档管理控制器属性
     * QML使用场景：主菜单/设置界面中读取存档、保存游戏进度，监听存档操作结果信号。
     */
    Q_PROPERTY(SaveManager* save READ save CONSTANT)

public:
    /**
     * @brief 构造函数（显式构造，禁止隐式转换）
     * @param parent 父对象指针（遵循Qt父子内存管理机制，避免内存泄漏）
     * 初始化逻辑：
     * 1. 实例化所有子模块（GameController/StoryManager/SaveManager）；
     * 2. 为子模块设置父对象（与AppController绑定生命周期）；
     * 3. 建立子模块间的关联（如GameController触发剧情时调用StoryManager）；
     * 4. 初始化全局默认状态（如默认导航到主菜单）。
     */
    explicit AppController(QObject *parent = nullptr);

    /**
     * @brief QML可调用的全局导航接口（Q_INVOKABLE标记后QML才能直接调用）
     * @param viewName 目标界面名称（与qml/views下的文件名对应，如"MainMenuView"、"GameView"）
     * 功能逻辑：
     * 1. 校验viewName的合法性（防止传入不存在的界面名称）；
     * 2. 发射viewChanged信号，通知QML端切换对应界面；
     * 3. 针对特殊界面做初始化（如进入GameView时重置棋盘，进入StoryView时加载初始剧情）；
     * 4. 打印导航日志（便于调试界面切换问题）。
     */
    Q_INVOKABLE void navigateTo(const QString& viewName);

    /**
     * @brief 游戏控制器的只读接口（Q_PROPERTY的READ函数）
     * @return GameController* 游戏逻辑控制器实例指针
     * 注意：返回的是全局唯一实例，QML调用的所有游戏逻辑都通过此指针触发。
     */
    GameController* game() const { return m_gameCtrl; }

    /**
     * @brief 剧情控制器的只读接口（Q_PROPERTY的READ函数）
     * @return StoryManager* 剧情管理控制器实例指针
     * 注意：返回的是全局唯一实例，QML调用的所有剧情逻辑都通过此指针触发。
     */
    StoryManager* story() const { return m_storyMgr; }

    /**
     * @brief 存档控制器的只读接口（Q_PROPERTY的READ函数）
     * @return SaveManager* 存档管理控制器实例指针
     * 注意：返回的是全局唯一实例，QML调用的所有存档逻辑都通过此指针触发。
     */
    SaveManager* save() const { return m_saveMgr; }

signals:
    /**
     * @brief 界面切换信号（C++→QML）
     * @param viewName 目标界面名称（QML端监听此信号，根据名称加载对应View组件）
     * 触发时机：调用navigateTo函数并校验合法后发射。
     */
    void viewChanged(const QString& viewName);

private:
    // 私有成员变量：存储子模块实例指针，仅在AppController内部管理
    /**
     * @brief 游戏逻辑控制器实例指针
     * 初始化规则：构造函数中new，析构函数中delete（或依赖Qt父子内存管理自动释放）
     */
    GameController* m_gameCtrl = nullptr;
    /**
     * @brief 剧情管理控制器实例指针
     * 初始化规则：构造函数中new，析构函数中delete（或依赖Qt父子内存管理自动释放）
     */
    StoryManager* m_storyMgr = nullptr;
    /**
     * @brief 存档管理控制器实例指针
     * 初始化规则：构造函数中new，析构函数中delete（或依赖Qt父子内存管理自动释放）
     */
    SaveManager* m_saveMgr = nullptr;
};

#endif // APPCONTROLLER_H
