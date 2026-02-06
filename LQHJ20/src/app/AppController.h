#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QTimer>

// 必须在头文件中声明枚举
enum ViewType {
    VIEW_MAIN_MENU,
    VIEW_GAME,
    VIEW_SETTING,
    VIEW_STORY
};

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ViewType currentView READ currentView WRITE setCurrentView NOTIFY currentViewChanged)

public:
    // 声明构造函数和析构函数
    explicit AppController(QObject *parent = nullptr);
    ~AppController() override;

    // 声明成员函数
    ViewType currentView() const { return m_currentView; }
    void setCurrentView(ViewType view);

    // 声明单例模式的静态成员
    static AppController* getInstance();

signals:
    void currentViewChanged(ViewType view);
    void showTipDialog(QString title, QString content);
    void showConfirmDialog(QString title, QString content);

public slots:
    // 声明所有槽函数
    void onStartGameClicked();
    void onBackToMenuClicked();
    void onGameOver(bool isWin);
    void onDialogConfirmed();
    void onDialogCanceled();

private:
    ViewType m_currentView = VIEW_MAIN_MENU;
    // 声明静态成员变量
    static AppController* m_instance;
};

#endif // APPCONTROLLER_H
