import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: dialog
    width: 300
    height: 180
    radius: 10
    color: "white"
    border.width: 1
    border.color: "#EEEEEE"
    visible: false  // 默认隐藏
    anchors.centerIn: parent  // 居中显示
    z: 9999  // 置顶显示，不被其他内容遮挡

    // 可外部自定义的属性
    property string titleText: "提示"
    property string contentText: ""
    property string confirmText: "确认"
    property string cancelText: "取消"
    property bool showCancelBtn: true

    // 弹出/隐藏动画
    NumberAnimation on scale { from: 0.8; to: 1.0; duration: 200; running: dialog.visible }
    NumberAnimation on opacity { from: 0; to: 1; duration: 200; running: dialog.visible }

    // 背景遮罩，防止误点
    Rectangle {
        id: mask
        anchors.fill: parent.parent
        color: "rgba(0,0,0,0.5)"
        visible: dialog.visible
        z: dialog.z - 1
    }

    // 对话框内容布局
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            Layout.fillWidth: true
            text: dialog.titleText
            font.pixelSize: 18
            font.bold: true
            color: "#333333"
        }

        Text {
            Layout.fillWidth: true
            text: dialog.contentText
            font.pixelSize: 16
            color: "#666666"
            wrapMode: Text.WordWrap  // 文字自动换行
        }

        // 按钮行
        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            CustomButton {
                id: cancelBtn
                visible: dialog.showCancelBtn
                text: dialog.cancelText
                btnWidth: 80
                btnHeight: 36
                btnColor: "#CCCCCC"  // 取消按钮用灰色，区分主次
                borderColor: "#AAAAAA"
                onClicked: { dialog.visible = false; dialog.canceled(); }
            }

            CustomButton {
                id: confirmBtn
                text: dialog.confirmText
                btnWidth: 80
                btnHeight: 36
                onClicked: { dialog.visible = false; dialog.confirmed(); }
            }
        }
    }

    signal confirmed()  // 确认按钮点击信号
    signal canceled()   // 取消按钮点击信号

    // 显示对话框的快捷函数
    function showDialog(title, content, confirmTxt = "确认", cancelTxt = "取消", showCancel = true) {
        dialog.titleText = title;
        dialog.contentText = content;
        dialog.confirmText = confirmTxt;
        dialog.cancelText = cancelTxt;
        dialog.showCancelBtn = showCancel;
        dialog.visible = true;
    }
}
