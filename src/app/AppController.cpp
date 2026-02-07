#include "AppController.h"

/**
 * @brief 构造函数实现：初始化子模块与全局状态
 * 实现步骤（组员需补充的逻辑）：
 * 1. 实例化三个核心子模块，将this作为父对象（利用Qt父子机制自动释放内存）；
 *    示例：m_gameCtrl = new GameController(this);
 * 2. 初始化子模块的默认状态（如SaveManager加载本地存档、StoryManager加载初始剧情配置）；
 * 3. 建立子模块间的信号槽关联（如GameController的gameOver信号触发StoryManager的nextChapter）；
 * 4. （可选）默认导航到主菜单界面（调用navigateTo("MainMenuView")）；
 * 5. 打印模块初始化日志（qInfo() << "AppController initialized successfully";）。
 */
AppController::AppController(QObject *parent)
    : QObject(parent)          // 调用父类QObject的构造函数
    , m_gameCtrl(new GameController(this))      // 初始化游戏控制器指针为nullptr
    , m_storyMgr(new StoryManager(this))      // 初始化剧情控制器指针为nullptr
    , m_saveMgr(new SaveManager(this))       // 初始化存档控制器指针为nullptr
{
    // 空实现：组员需按上述步骤补充代码
    navigateTo("MainMenuView");

    qInfo() << "AppController 初始化完成，子模块已实例化";
}

/**
 * @brief 全局导航函数空实现
 * 详细实现逻辑（组员需补充的核心步骤）：
 * Step1：合法性校验
 * - 判断viewName是否为空，或是否是支持的界面名称（可在utils/Constants.h中定义支持的界面列表）；
 * - 若不合法，打印警告日志（qWarning() << "Invalid view name:" << viewName;）并直接返回。
 *
 * Step2：特殊界面初始化（根据viewName做前置处理）
 * - 若viewName == "GameView"：调用m_gameCtrl->resetGame()重置棋盘和回合；
 * - 若viewName == "StoryView"：调用m_storyMgr->loadInitialChapter()加载初始剧情章节；
 * - 若viewName == "SettingsView"：调用m_saveMgr->loadSettings()加载本地设置（音量、棋盘大小）。
 *
 * Step3：发射界面切换信号
 * - 调用emit viewChanged(viewName)，通知QML端切换界面。
 *
 * Step4：打印导航日志
 * - 用qInfo()打印"Navigate to:" << viewName，便于调试。
 *
 * @param viewName 目标界面名称（与qml/views下的文件basename一致，如"GameView"对应GameView.qml）
 */
void AppController::navigateTo(const QString& viewName)
{
    // 空实现：组员需按上述步骤补充代码
}
