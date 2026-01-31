#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

// 只引入必须的头文件
#include "app/AppController.h"

int main(int argc, char *argv[])
{
    // 1. 初始化Qt应用（高DPI适配：Qt6后AA_EnableHighDpiScaling已废弃，不用加）
    QGuiApplication app(argc, argv);

    // 2. 创建全局唯一的AppController（必须在这里new，或者栈上实例化，确保生命周期）
    // 注意：如果是栈上实例化，要确保在engine.load之前
    AppController appController;

    // 3. 初始化QML引擎
    QQmlApplicationEngine engine;

    // 4. 将AppController暴露给QML（QML中通过app调用所有C++接口）
    engine.rootContext()->setContextProperty("app", &appController);

    // 5. 加载Main.qml
    const QUrl mainQmlUrl(QStringLiteral("qrc:/qml/qml/Main.qml"));

    // 监听QML加载失败信号（调试用）
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [mainQmlUrl](QObject *obj, const QUrl &objUrl) {
                         if (!obj && mainQmlUrl == objUrl) {
                             qCritical() << "加载QML失败：找不到" << mainQmlUrl;
                             QCoreApplication::exit(-1);
                         } else if (obj) {
                             qInfo() << "QML加载成功：" << objUrl;
                         }
                     }, Qt::QueuedConnection);

    // 执行加载
    engine.load(mainQmlUrl);

    // 6. 启动应用事件循环
    return app.exec();
}
