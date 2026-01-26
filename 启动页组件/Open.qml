import QtQuick 2.16
import QtQuick.Window 2.16
import QtQuick.Controls 2.16
import QtQuick.Layouts 1.16
import QtGraphicalEffects 1.16
import QtQuick.Shapes 1.16

ApplicationWindow {
    id: splashWindow
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true
    title: "五子棋·弈境"
    // 无框窗口+窗口基础属性（Qt6专属flags）
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint
    color: "transparent"
    // 全局字体：现代思源黑体，抗锯齿
    font.family: "思源黑体 Medium"
    font.pixelSize: 16
    font.styleName: "Medium"

    // 背景层：棋盘木色渐变，带轻微噪点纹理（现代质感）
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#E8D9B5" }
            GradientStop { position: 1.0; color: "#D4B886" }
        }
        // 轻微纹理叠加，避免背景单调
        layer.enabled: true
        layer.effect: NoiseEffect {
            noiseType: NoiseEffect.Simplex
            scale: 50
            opacity: 0.05
        }
        // 背景淡入动画
        opacity: 0
        PropertyAnimation on opacity {
            to: 1
            duration: 800
            easing.type: Easing.OutCubic
        }
    }

    // 右上角关闭按钮：现代简约，轻交互
    Button {
        id: closeBtn
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 30
        width: 40
        height: 40
        radius: 20
        flat: true
        // 关闭图标（原生Shape，无需图片）
        contentItem: Shape {
            anchors.fill: parent
            ShapePath {
                strokeWidth: 2
                strokeColor: "#333333"
                strokeCap: ShapePath.RoundCap
                fillColor: "transparent"
                PathLine { x: parent.width; y: parent.height }
                PathMove { x: parent.width; y: 0 }
                PathLine { x: 0; y: parent.height }
            }
        }
        // 按钮交互动效
        background: Rectangle {
            color: closeBtn.hovered ? "#F5F5F5" : "transparent"
            radius: 20
            opacity: closeBtn.hovered ? 0.7 : 0
            PropertyAnimation on opacity { duration: 200; easing.type: Easing.OutCubic }
        }
        // 关闭窗口信号
        onClicked: splashWindow.close()
        // 延迟淡入
        opacity: 0
        SequentialAnimation on opacity {
            delay: 1200
            PropertyAnimation { to: 1; duration: 500; easing.type: Easing.OutCubic }
        }
    }

    // 中间主内容容器：标题+棋子+开始按钮
    ColumnLayout {
        id: mainContainer
        anchors.centerIn: parent
        spacing: 60
        Layout.alignment: Qt.AlignCenter

        // 游戏标题：五子棋·弈境（结合剧情感，弈境=对弈之境）
        Text {
            id: gameTitle
            text: "五子棋 · 弈境"
            font.family: "思源黑体 Bold"
            font.pixelSize: 80
            color: "#2D2D2D"
            horizontalAlignment: Text.AlignHCenter
            // 标题阴影（提升层次感）
            layer.enabled: true
            layer.effect: DropShadow {
                color: "#00000066"
                radius: 8
                offset.x: 3
                offset.y: 3
                samples: 16
            }
            // 标题核心动效：淡入+缩放，丝滑缓动
            opacity: 0
            scale: 0.8
            ParallelAnimation {
                id: titleAni
                PropertyAnimation { target: gameTitle; property: "opacity"; to: 1; duration: 1000; easing.type: Easing.OutElastic }
                PropertyAnimation { target: gameTitle; property: "scale"; to: 1; duration: 1000; easing.type: Easing.OutElastic; easing.amplitude: 0.3 }
            }
            Component.onCompleted: titleAni.start()
        }

        // 黑白棋子容器：左右分布，带飞入+循环浮动动效
        RowLayout {
            id: chessContainer
            Layout.alignment: Qt.AlignCenter
            spacing: 80
            // 黑棋（拟真渐变，五子棋经典）
            Rectangle {
                id: blackChess
                width: 60
                height: 60
                radius: 30
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#2A2A2A" }
                    GradientStop { position: 0.8; color: "#0A0A0A" }
                }
                border.color: "#1A1A1A"
                border.width: 1
                // 棋子阴影
                layer.enabled: true
                layer.effect: DropShadow {
                    color: "#00000088"
                    radius: 6
                    offset.x: 2
                    offset.y: 2
                    samples: 12
                }
                // 黑棋动效：左侧飞入+循环上下浮动+轻微旋转
                x: -100
                opacity: 0
                ParallelAnimation {
                    id: blackChessAni
                    PropertyAnimation { target: blackChess; property: "x"; to: 0; duration: 800; delay: 300; easing.type: Easing.OutCubic }
                    PropertyAnimation { target: blackChess; property: "opacity"; to: 1; duration: 800; delay: 300; easing.type: Easing.OutCubic }
                }
                // 循环浮动动画
                SequentialAnimation on y {
                    loops: Animation.Infinite
                    PropertyAnimation { to: -10; duration: 1500; easing.type: Easing.InOutSine }
                    PropertyAnimation { to: 10; duration: 1500; easing.type: Easing.InOutSine }
                }
                RotationAnimation on rotation {
                    loops: Animation.Infinite
                    from: 0
                    to: 360
                    duration: 10000
                    direction: Animation.Counterclockwise
                }
                Component.onCompleted: blackChessAni.start()
            }

            // 白棋（拟真渐变，与黑棋对称）
            Rectangle {
                id: whiteChess
                width: 60
                height: 60
                radius: 30
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#FFFFFF" }
                    GradientStop { position: 0.8; color: "#F0F0F0" }
                }
                border.color: "#E0E0E0"
                border.width: 1
                // 棋子阴影
                layer.enabled: true
                layer.effect: DropShadow {
                    color: "#00000044"
                    radius: 6
                    offset.x: 2
                    offset.y: 2
                    samples: 12
                }
                // 白棋动效：右侧飞入+循环上下浮动+轻微旋转（与黑棋反向）
                x: 100
                opacity: 0
                ParallelAnimation {
                    id: whiteChessAni
                    PropertyAnimation { target: whiteChess; property: "x"; to: 0; duration: 800; delay: 300; easing.type: Easing.OutCubic }
                    PropertyAnimation { target: whiteChess; property: "opacity"; to: 1; duration: 800; delay: 300; easing.type: Easing.OutCubic }
                }
                // 循环浮动动画（与黑棋反向，更有韵律）
                SequentialAnimation on y {
                    loops: Animation.Infinite
                    PropertyAnimation { to: 10; duration: 1500; easing.type: Easing.InOutSine }
                    PropertyAnimation { to: -10; duration: 1500; easing.type: Easing.InOutSine }
                }
                RotationAnimation on rotation {
                    loops: Animation.Infinite
                    from: 0
                    to: 360
                    duration: 10000
                    direction: Animation.Clockwise
                }
                Component.onCompleted: whiteChessAni.start()
            }
        }

        // 开始游戏按钮：现代圆角，木色主调，强交互动效
        Button {
            id: startBtn
            text: "开始弈境"
            font.family: "思源黑体 Medium"
            font.pixelSize: 32
            color: "#2D2D2D"
            width: 280
            height: 80
            radius: 40
            // 按钮背景：渐变木色，按压效果
            background: Rectangle {
                color: "transparent"
                gradient: Gradient {
                    GradientStop { position: 0.0; color: startBtn.pressed ? "#C8A878" : (startBtn.hovered ? "#DCC8A8" : "#E8D9B5") }
                    GradientStop { position: 1.0; color: startBtn.pressed ? "#B89868" : (startBtn.hovered ? "#D4B886" : "#E0D0A0") }
                }
                radius: 40
                border.color: "#2D2D2D"
                border.width: 2
                // 按钮阴影
                layer.enabled: true
                layer.effect: DropShadow {
                    color: "#00000044"
                    radius: 10
                    offset.x: 4
                    offset.y: 4
                    samples: 16
                }
                // 按压缩放效果
                scale: startBtn.pressed ? 0.95 : 1
                PropertyAnimation on scale { duration: 200; easing.type: Easing.OutCubic }
            }
            // 按钮核心动效：延迟淡入+缩放，在棋子动画后出现
            opacity: 0
            scale: 0.9
            ParallelAnimation {
                id: startBtnAni
                delay: 1200
                PropertyAnimation { target: startBtn; property: "opacity"; to: 1; duration: 800; easing.type: Easing.OutCubic }
                PropertyAnimation { target: startBtn; property: "scale"; to: 1; duration: 800; easing.type: Easing.OutElastic; easing.amplitude: 0.3 }
            }
            // 点击信号：对接C++游戏主界面（关键信号）
            onClicked: {
                // 点击后淡出动画，然后触发C++信号
                ParallelAnimation {
                    PropertyAnimation { target: mainContainer; property: "opacity"; to: 0; duration: 500; easing.type: Easing.OutCubic }
                    PropertyAnimation { target: mainContainer; property: "scale"; to: 0.9; duration: 500; easing.type: Easing.OutCubic }
                    PropertyAnimation { target: splashWindow; property: "opacity"; to: 0; duration: 600; easing.type: Easing.OutCubic }
                    onRunningChanged: {
                        if (!running) {
                            // 此处触发C++信号，示例：gameEngine.startGame()
                            splashWindow.close()
                        }
                    }
                }.start()
            }
            Component.onCompleted: startBtnAni.start()
        }
    }

    // 底部版本/版权信息：轻量淡入
    Text {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        text: "弈境 v1.0 | Qt6.8 开发"
        font.pixelSize: 14
        color: "#666666"
        opacity: 0
        PropertyAnimation on opacity {
            to: 0.8
            duration: 800
            delay: 1500
            easing.type: Easing.OutCubic
        }
    }
}
