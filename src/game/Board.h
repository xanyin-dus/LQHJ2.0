#pragma once
#ifndef BOARD_H  // 修正原宏定义笔误：BORD_H → BOARD_H
#define BOARD_H

#include <vector>
#include "../utils/Utils.h"    // 工具类（坐标校验、日志等）
#include "../story/Constants.h"// 全局配置（棋盘大小、棋子类型等，修正原路径错误：../story/Constants.h → ../utils/Constants.h）

/**
 * @brief 五子棋棋盘核心逻辑类
 * 核心职责：
 * 1. 维护15×15棋盘的状态（每个位置的棋子类型）；
 * 2. 处理落子合法性校验（位置是否在棋盘内、是否为空）；
 * 3. 实现五子连珠的胜负判断（横、竖、斜四个方向）；
 * 4. 提供棋盘重置、状态查询等基础接口。
 * 设计特点：纯逻辑类（不继承QObject），仅负责棋盘数据与规则，与UI层解耦。
 */
class Board {
public:
    /**
     * @brief 构造函数
     * 初始化逻辑：将棋盘所有位置初始化为空棋子（PieceType::None）。
     */
    Board();

    /**
     * @brief 重置棋盘
     * 功能：将棋盘所有位置恢复为空状态，用于新局开始或游戏重置。
     */
    void reset();

    /**
     * @brief 落子操作
     * @param row 目标行坐标（范围：0~Config::BOARD_SIZE-1）
     * @param col 目标列坐标（范围：0~Config::BOARD_SIZE-1）
     * @param type 棋子类型（黑棋/白棋）
     * @return bool 落子结果：true=落子成功（位置合法且为空），false=落子失败（位置越界或已有棋子）
     */
    bool placePiece(int row, int col, Config::PieceType type);

    /**
     * @brief 获取指定位置的棋子类型
     * @param row 行坐标
     * @param col 列坐标
     * @return Config::PieceType 棋子类型：若位置越界返回PieceType::None
     * @note const修饰：函数不修改棋盘状态，仅做查询。
     */
    Config::PieceType getPiece(int row, int col) const;

    /**
     * @brief 判断指定位置落子后是否获胜
     * @param row 落子的行坐标
     * @param col 落子的列坐标
     * @param type 落子的棋子类型
     * @return bool 胜负结果：true=形成五子连珠，false=未获胜
     * 核心逻辑：检查落子点的**横、竖、左上→右下、右上→左下**四个方向，是否存在连续5枚同色棋子。
     */
    bool checkWin(int row, int col, Config::PieceType type);

    /**
     * @brief 检查棋盘是否已满（平局判断）
     * @return bool 结果：true=棋盘无空位置（平局），false=仍有空位置
     * @note const修饰：仅查询状态，不修改棋盘。
     */
    bool isFull() const;

private:
    /**
     * @brief 棋盘状态存储
     * 15×15的二维数组，每个元素存储对应位置的棋子类型（None/Black/White）。
     * 初始化时所有元素为PieceType::None。
     */
    Config::PieceType m_grid[Config::BOARD_SIZE][Config::BOARD_SIZE];
};

#endif // BOARD_H
