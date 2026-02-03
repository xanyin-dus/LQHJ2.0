#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <QPoint>
#include <QString>
#include <QDebug>
#include <QDateTime> // 新增：日志添加时间戳所需

// 引入全局常量，坐标转换依赖CELL_SIZE（格子像素大小）
#include "../story/Constants.h"

/**
 * @brief 全局通用工具类
 * 核心职责：
 * 1. 坐标转换：实现QML鼠标点击的像素坐标与棋盘网格索引（0~14）的双向转换；
 * 2. 日志封装：提供统一格式的调试日志输出（含时间戳、模块名、日志级别）；
 * 3. （后续扩展）提供字符串处理、数据校验等通用工具函数。
 * 设计特点：纯静态工具类，无成员变量，所有函数均为static；私有化构造函数，禁止实例化；不依赖Qt信号槽，轻量高效。
 */
class Utils {
public:
    /**
     * @brief 像素坐标转棋盘网格索引（核心功能）
     * 将QML中鼠标点击的屏幕像素坐标（x,y）转换为棋盘的网格索引（row,col），范围0~14。
     * @param mouseX 鼠标点击的X像素坐标（相对于GameView的左上角）
     * @param mouseY 鼠标点击的Y像素坐标（相对于GameView的左上角）
     * @return QPoint 棋盘网格坐标：x=列索引col，y=行索引row；若转换后越界，返回QPoint(-1,-1)
     * 转换逻辑：(像素坐标 - 棋盘偏移) / 格子像素大小（CELL_SIZE），并做边界校验。
     * 依赖常量：Config::CELL_SIZE（格子大小）、Config::BOARD_SIZE（棋盘大小）。
     */
    static QPoint pixelToGrid(int mouseX, int mouseY);

    /**
     * @brief 棋盘网格索引转像素坐标（核心功能）
     * 将棋盘的网格索引（row,col）转换为QML中棋子显示的中心点像素坐标（x,y）。
     * @param row 棋盘行索引（0~14）
     * @param col 棋盘列索引（0~14）
     * @return QPoint 棋子中心点的像素坐标：x=列像素坐标，y=行像素坐标；若索引越界，返回QPoint(-1,-1)
     * 转换逻辑：网格索引 * 格子像素大小 + 格子大小/2（中心点偏移），并做边界校验。
     * 依赖常量：Config::CELL_SIZE（格子大小）、Config::BOARD_SIZE（棋盘大小）。
     */
    static QPoint gridToPixel(int row, int col);

    /**
     * @brief 统一日志输出函数（基础版）
     * 输出带时间戳、模块名、默认INFO级别的调试日志，格式统一，便于开发排错。
     * @param module 日志所属模块名（如"GameController"、"StoryManager"）
     * @param message 日志具体内容（如"落子成功：行10列5"）
     * 输出格式：[yyyy-MM-dd hh:mm:ss] [模块名] [INFO] 日志内容。
     */
    static void log(const QString& module, const QString& message);

    /**
     * @brief 带级别的日志输出函数（重载版，新增）
     * 支持自定义日志级别（INFO/ERROR/WARN），适配不同调试场景。
     * @param module 日志所属模块名
     * @param message 日志具体内容
     * @param level 日志级别（默认INFO，可选"ERROR"/"WARN"/"INFO"）
     */
    static void log(const QString& module, const QString& message, const QString& level);

private:
    /**
     * @brief 私有构造函数（禁止实例化）
     * 工具类仅提供静态函数，无需实例化，私有化构造函数防止外部创建对象。
     */
    Utils() = default;

    /**
     * @brief 私有析构函数（禁止析构）
     * 配合私有构造函数，确保工具类完全无法实例化。
     */
    ~Utils() = default;

    // 禁用拷贝构造和赋值运算符（防止通过拷贝实例化）
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;
};

#endif // UTILS_H
