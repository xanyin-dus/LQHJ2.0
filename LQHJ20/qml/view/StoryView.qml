import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import"../components"
Rectangle {
    id: root
    anchors.fill: parent
    color: "#303030"  // 深色背景，突出剧情文本

    Image { anchors.fill: parent; source: root.bgImage; fillMode: Image.PreserveAspectCrop; opacity: 0.3 }

    // 剧情文本框
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 200
        color: "rgba(0,0,0,0.7)"
        radius: 10

        Text {
            anchors.fill: parent
            anchors.margins: 20
            text: root.storyText
            font.pixelSize: 18
            color: "white"
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignTop
        }
    }

    // 选项按钮
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        spacing: 15

        Repeater {
            model: root.optionList
            CustomButton {
                text: modelData.text
                btnWidth: 120
                btnHeight: 36
                onClicked: root.optionSelected(modelData.id)
            }
        }
    }

    // 可外部设置的属性
    property string storyText: ""
    property string bgImage: ""
    property var optionList: []

    // 供外部连接的信号
    signal optionSelected(int optionId)
    signal backToMenuClicked()
}
