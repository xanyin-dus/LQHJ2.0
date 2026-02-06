import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import"../components"
Rectangle {
    id: root
    anchors.fill: parent
    color: "#FFF8E1"  // 米黄色背景，温馨护眼

    // 标题
    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 80
        text: "五子棋·寻梦"
        font.pixelSize: 48
        font.bold: true
        color: "#E67300"  // 暖橙色标题，醒目
    }

    // 按钮布局
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 25

        CustomButton { text: "开始游戏"; btnWidth: 200; btnHeight: 50; onClicked: root.startGameClicked() }
        CustomButton { text: "剧情模式"; btnWidth: 200; btnHeight: 50; onClicked: root.storyModeClicked() }
        CustomButton { text: "游戏设置"; btnWidth: 200; btnHeight: 50; onClicked: root.settingsClicked() }
        CustomButton { text: "退出游戏"; btnWidth: 200; btnHeight: 50; btnColor: "#FF6B6B"; borderColor: "#E53935"; onClicked: root.exitClicked() }
    }

    // 供外部连接的信号
    signal startGameClicked()
    signal storyModeClicked()
    signal settingsClicked()
    signal exitClicked()
}
