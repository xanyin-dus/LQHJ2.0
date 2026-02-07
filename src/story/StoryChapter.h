#pragma once
#ifndef STORYCHAPTER_H
#define STORYCHAPTER_H

#include <QString>
#include <QList>
#include "../story/Constants.h" // 引入全局路径常量，规范资源路径格式

/**
 * @brief 剧情分支选项结构体
 * 核心作用：定义剧情节点的可选分支，玩家选择后会跳转到指定的剧情帧。
 * 每个选项包含“显示文本”和“目标对话ID”，是剧情分支的最小单元。
 */
struct StoryOption {
    /**
     * @brief 选项显示文本（玩家可见）
     * 示例：“挑战五子棋高手”、“选择离开茶馆”
     */
    QString text;

    /**
     * @brief 选择该选项后跳转的目标剧情帧ID
     * 需与目标 StoryFrame 的 id 字段完全一致，用于 StoryManager 定位下一个剧情节点。
     * 示例：“frame_005”、“branch_end”
     */
    QString jumpToID;
};

/**
 * @brief 剧情帧结构体（剧情的基本单元）
 * 核心作用：定义一段完整的剧情节点，包含对话内容、视觉资源、背景音乐和分支选项，是剧情配置的核心数据结构。
 * 每个剧情帧对应 StoryView 中的一屏剧情展示，玩家点击“继续”或选择选项后进入下一个剧情帧。
 */
struct StoryFrame {
    /**
     * @brief 剧情帧唯一ID（全局唯一）
     * 用于剧情跳转、存档标记的核心标识，格式建议统一（如“chapter_1_frame_001”）。
     * 示例：“prologue_001”、“win_story_003”
     */
    QString id;

    /**
     * @brief 说话人名称（剧情文本的发言角色）
     * 示例：“老者”、“主角”、“AI棋手”
     * 作用：在 StoryView 中显示角色名称，区分不同发言者。
     */
    QString speaker;

    /**
     * @brief 剧情对话文本（核心内容）
     * 支持换行（用“\n”分隔），示例：“年轻人，要不要来一局五子棋？\n我可是这一带的高手。”
     */
    QString text;

    /**
     * @brief 剧情帧背景图路径（基于res/images/story/）
     * 路径需遵循 Constants.h 中的资源约定，示例：“qrc:/res/images/story/bg_teahouse.png”
     * 作用：在 StoryView 中作为当前剧情的背景，增强沉浸感。
     */
    QString bgImage;

    /**
     * @brief 剧情帧背景音乐路径（基于res/audio/story/）
     * 路径需遵循 Constants.h 中的资源约定，示例：“qrc:/res/audio/story/bgm_peace.mp3”
     * 作用：切换当前剧情的背景音乐，配合氛围变化（如紧张/温馨场景）。
     */
    QString bgm;

    /**
     * @brief 剧情分支选项列表
     * - 非空：显示选项按钮，玩家选择后跳转到对应 jumpToID 的剧情帧；
     * - 为空：仅显示“继续”按钮，点击后自动跳转到下一个顺序剧情帧（由 StoryManager 管理顺序）。
     * 示例：包含两个 StoryOption，分别对应“接受挑战”和“拒绝挑战”分支。
     */
    QList<StoryOption> options;
};

#endif // STORYCHAPTER_H
