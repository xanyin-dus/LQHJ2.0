#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Config {
// 棋盘配置
constexpr int BOARD_SIZE = 15;      // 15x15
constexpr int CELL_SIZE = 40;       // 格子像素大小（用于界面计算）


// 棋子类型枚举
enum class PieceType { None, Black, White };

// 游戏结果状态
enum class GameState { Playing, BlackWin, WhiteWin, Draw };

// 路径配置 (适配 qml.qrc)
const QString IMG_PATH = "qrc:/res/images/";
const QString AUDIO_PATH = "qrc:/res/audio/";
}

#endif // CONSTANTS_H
