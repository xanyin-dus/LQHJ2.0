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

const QString STORY_PATH = ":/res/story/"; // 剧情JSON文件根路径
const QString STORY_IMAGE_PATH = "qrc:/res/res/images/story/"; // 剧情背景图路径
const QString STORY_AUDIO_PATH = "qrc:/res/audio/story/"; // 剧情BGM/音效路径
}



#endif // CONSTANTS_H
