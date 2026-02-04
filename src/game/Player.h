#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "../story/Constants.h"

/**
 * @brief 玩家信息封装类
 * 核心职责：
 * 1. 存储玩家的核心属性：名称、棋子颜色、玩家类型（人类/AI）；
 * 2. 提供只读属性访问接口（getter），确保数据安全；
 * 3. 提供AI玩家判断接口，支持GameController区分人类与AI行动。
 * 设计特点：纯数据类（无业务逻辑）、成员变量私有（仅通过getter访问）、支持默认构造与带参构造，符合C++封装性原则。
 */
class Player
{
public:
    /**
     * @brief 玩家类型枚举
     * 用于区分不同类型的玩家，支持后续扩展更多AI难度等级。
     * - Human：人类玩家（通过QML界面输入落子）；
     * - AI_Easy：简单AI（随机落子，适合新手）；
     * - AI_Hard：困难AI（极大极小值算法，适合进阶玩家）。
     */
    enum class Type { Human, AI_Easy, AI_Hard };

    /**
     * @brief 默认构造函数
     * 初始化玩家为默认状态：名称“未知玩家”、棋子颜色为空、类型为人类玩家。
     * 作用：避免未初始化的成员变量导致空指针/非法值，提升代码健壮性。
     */
    Player();

    /**
     * @brief 带参数构造函数
     * 用传入的参数初始化玩家属性，支持创建自定义配置的玩家。
     * @param name 玩家名称（如“黑方”“白方”“AI（简单）”）；
     * @param color 玩家对应的棋子颜色（黑棋/白棋）；
     * @param type 玩家类型（人类/AI_Easy/AI_Hard）。
     */
    Player(QString name, Config::PieceType color, Type type);

    /**
     * @brief 获取玩家名称的只读接口
     * @return QString 玩家名称（如“黑方”）。
     * @note const修饰：函数不修改成员变量，仅做查询。
     */
    QString name() const;

    /**
     * @brief 获取玩家棋子颜色的只读接口
     * @return Config::PieceType 棋子颜色（Black/White/None）。
     * @note const修饰：函数不修改成员变量，仅做查询。
     */
    Config::PieceType color() const;

    /**
     * @brief 判断当前玩家是否为AI的接口
     * @return bool 结果：true=AI玩家，false=人类玩家。
     * 核心逻辑：检查玩家类型是否为AI_Easy或AI_Hard。
     * @note const修饰：函数不修改成员变量，仅做查询。
     */
    bool isAI() const;

    /**
     * @brief 设置玩家类型（人类 / 各难度 AI）
     * 提供受控写接口，供 GameController 在不同模式下切换玩家类型。
     */
    void setType(Type type);

    /**
     * @brief 获取玩家当前类型
     */
    Type type() const { return m_type; }

private:
    /**
     * @brief 玩家名称（如“黑方”“白方”）
     * 初始化：默认构造设为“未知玩家”，带参构造由传入参数赋值。
     */
    QString m_name;

    /**
     * @brief 玩家对应的棋子颜色
     * 初始化：默认构造设为PieceType::None，带参构造由传入参数赋值。
     */
    Config::PieceType m_color;

    /**
     * @brief 玩家类型（人类/AI）
     * 初始化：默认构造设为Type::Human，带参构造由传入参数赋值。
     */
    Type m_type;
};

#endif // PLAYER_H
