#include "AppController.h"
#include <QDebug>
#include <QCoreApplication>

AppController* AppController::m_instance = nullptr;

// 构造函数
AppController::AppController(QObject *parent) : QObject(parent)
{
    qDebug() << "AppController initialized";
}

// 析构函数
AppController::~AppController()
{
    m_instance = nullptr;
}

// 单例模式，全局唯一实例
AppController* AppController::getInstance()
{
    if (!m_instance) {
        m_instance = new AppController();
    }
    return m_instance;
}

// 设置当前显示的界面
void AppController::setCurrentView(ViewType view)
{
    if (m_currentView != view) {
        m_currentView = view;
        emit currentViewChanged(view);  // 发出界面切换信号
    }
}

// 处理开始游戏点击
void AppController::onStartGameClicked()
{
    setCurrentView(VIEW_GAME);
    emit showTipDialog("游戏开始", "祝您玩得开心！");
}

// 处理返回菜单点击
void AppController::onBackToMenuClicked()
{
    emit showConfirmDialog("返回菜单", "确定要返回吗？");
}

// 处理游戏结束
void AppController::onGameOver(bool isWin)
{
    QString title = isWin ? "恭喜胜利！" : "游戏结束";
    QString content = isWin ? "您真棒！" : "再接再厉！";
    emit showTipDialog(title, content);
    QTimer::singleShot(3000, this, [=](){ setCurrentView(VIEW_MAIN_MENU); });
}

// 对话框确认按钮回调
void AppController::onDialogConfirmed()
{
    if (m_currentView == VIEW_GAME || m_currentView == VIEW_STORY || m_currentView == VIEW_SETTING) {
        setCurrentView(VIEW_MAIN_MENU);
    } else if (m_currentView == VIEW_MAIN_MENU) {
        QCoreApplication::quit();  // 退出游戏
    }
}

// 对话框取消按钮回调
void AppController::onDialogCanceled()
{
    qDebug() << "Dialog canceled";
}
