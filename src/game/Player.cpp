#include "Player.h"

/**
 * @brief 默认构造函数实现
 * 用初始化列表为成员变量赋默认值，避免未初始化导致的空指针或非法值，符合防御性编程原则。
 * 默认值说明：
 * - m_name：“未知玩家”（避免空字符串）；
 * - m_color：PieceType::None（避免非法颜色值）；
 * - m_type：Type::Human（默认创建人类玩家）。
 */
Player::Player()
    : m_name("未知玩家")
    , m_color(Config::PieceType::None)
    , m_type(Type::Human)
{
}

/**
 * @brief 带参数构造函数实现
 * 用初始化列表直接初始化成员变量，效率更高（避免默认构造+赋值的额外开销），符合C++现代编程规范。
 * @param name 玩家名称；
 * @param color 棋子颜色；
 * @param type 玩家类型。
 */
Player::Player(QString name, Config::PieceType color, Player::Type type)
    : m_name(name)
    , m_color(color)
    , m_type(type)
{
}

/**
 * @brief 判断是否为AI玩家的实现
 * 核心逻辑：检查玩家类型是否为AI_Easy或AI_Hard，只要满足其一则判定为AI玩家。
 * @return bool AI玩家判断结果。
 */
bool Player::isAI() const {
    return m_type == Type::AI_Easy || m_type == Type::AI_Hard;
}

/**
 * @brief 获取玩家名称的实现
 * 直接返回私有成员m_name的值，确保数据只读性（外部无法修改名称）。
 * @return QString 玩家名称。
 */
QString Player::name() const {
    return m_name;
}

/**
 * @brief 获取棋子颜色的实现
 * 直接返回私有成员m_color的值，确保数据只读性（外部无法修改颜色）。
 * @return Config::PieceType 棋子颜色。
 */
Config::PieceType Player::color() const {
    return m_color;
}
