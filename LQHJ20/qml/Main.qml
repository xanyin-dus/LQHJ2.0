import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
// 如果是自定义组件
import "components"
import "view"


Window {
    id: rootWindow
    width: 800
    height: 600
    visible: true
    title: "五子棋游戏"
    color: "#F5F5F5"

    // 引入商参的控制器
    property var appController: QtObject { objectName: "AppController" }

    // 萧萧的对话框
    Dialog {
        id: gameDialog
        anchors.centerIn: parent
        onConfirmed: appController.onDialogConfirmed()
        onCanceled: appController.onDialogCanceled()
    }

    // 萧萧的主菜单
    MainMenuView {
        id: mainMenuView
        anchors.fill: parent
        visible: appController.currentView === 0
        onStartGameClicked: appController.onStartGameClicked()
    }

    // 萧萧的游戏界面
    GameView {
        id: gameView
        anchors.fill: parent
        visible: appController.currentView === 1
        onBackToMenuClicked: appController.onBackToMenuClicked()
        onGameOver: appController.onGameOver(isWin)
    }

    // 萧萧的设置界面
    SettingsView {
        id: settingsView
        anchors.fill: parent
        visible: appController.currentView === 2
    }

    // 萧萧的剧情界面
    StoryView {
        id: storyView
        anchors.fill: parent
        visible: appController.currentView === 3
    }

    // 监听控制器信号，切换界面
    Connections {
        target: appController
        function onCurrentViewChanged(view) {
            mainMenuView.visible = (view === 0);
            gameView.visible = (view === 1);
            settingsView.visible = (view === 2);
            storyView.visible = (view === 3);
        }
        function onShowTipDialog(title, content) {
            gameDialog.showDialog(title, content, "确认", "", false)
        }
        function onShowConfirmDialog(title, content) {
            gameDialog.showDialog(title, content)
        }
    }
}
