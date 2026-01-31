import LQHJ20 1.0
import QtQuick.Window
import QtQuick
import QtQuick.Controls

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("LQHJ20 黑白棋")

    // 核心：页面加载器（默认显示主菜单）
    Loader {
        id: pageLoader
        anchors.fill: parent
        source: "qrc:/LQHJ20/qml/view/MainMenuView.qml"
    }


    // 监听AppController的页面切换信号
    Connections {
        target: app
        function onViewChanged(viewName) {
            // 拼接完整的qrc路径，且强制统一文件名（如viewName传"Story"，拼出来就是StoryView.qml）
            const targetPath = "qrc:/qml/view/" + viewName + "View.qml";
            console.log("切换页面到：", targetPath); // 调试用，看路径对不对
            pageLoader.source = targetPath;
        }
    }
}
