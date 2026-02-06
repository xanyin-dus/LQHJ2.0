import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/components"

Rectangle {
    id: root
    // 可外部修改的属性，方便复用
    property string text: "按钮"
    property int btnWidth: 120
    property int btnHeight: 40
    property color btnColor: "#FF9933"  // 暖橙色按钮，奶奶看得清楚
    property color borderColor: "#E67300"

    width: btnWidth
    height: btnHeight
    color: btnColor
    radius: 8  // 圆角设计，不刺眼
    border.width: 1
    border.color: borderColor

    // 按钮文字，居中显示
    Text {
        anchors.centerIn: parent
        text: root.text
        font.pixelSize: 16  // 大字体，方便阅读
        font.bold: true
        color: "white"
    }

    // 点击交互区域
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor  // 鼠标变成小手，提示可点击
        onPressed: { root.scale = 0.95; root.color = borderColor; }  // 按下时缩小+变暗
        onReleased: { root.scale = 1.0; root.color = btnColor; }    // 松开时恢复原样
        onClicked: root.clicked()  // 点击时发出信号
    }

    signal clicked()  // 供外部连接的点击信号
}
