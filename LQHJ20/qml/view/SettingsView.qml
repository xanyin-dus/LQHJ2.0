import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import"../components"
Rectangle {
    id: root
    anchors.fill: parent
    color: "#F5F5F5"

    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 40
        text: "游戏设置"
        font.pixelSize: 36
        font.bold: true
        color: "#333333"
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 30

        // 音量设置
        RowLayout {
            Layout.fillWidth: true
            spacing: 15
            Text { text: "背景音乐音量"; font.pixelSize: 18; color: "#666666" }
            Slider { Layout.fillWidth: true; from: 0; to: 100; value: root.bgMusicVolume; onValueChanged: root.bgMusicVolume = value }
        }

        // 棋盘大小设置
        RowLayout {
            Layout.fillWidth: true
            spacing: 15
            Text { text: "棋盘大小"; font.pixelSize: 18; color: "#666666" }
            ComboBox { Layout.fillWidth: true; model: ["13×13", "15×15", "19×19"]; currentIndex: root.boardSizeIndex; onCurrentIndexChanged: root.boardSizeIndex = currentIndex }
        }

        CustomButton { text: "保存设置"; btnWidth: 160; btnHeight: 40; onClicked: root.settingsSaved() }
    }

    // 可外部设置的属性
    property int bgMusicVolume: 80
    property int boardSizeIndex: 1

    // 供外部连接的信号
    signal settingsSaved()
    signal backToMenuClicked()
}
