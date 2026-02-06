import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import"../components"
Rectangle {
    id: root
    anchors.fill: parent
    color: "#E8F5E9"  // 浅绿色背景，护眼

    // 顶部导航栏
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        color: "#FF9933"

        CustomButton {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 20
            text: "返回"
            btnWidth: 80
            btnHeight: 36
            btnColor: "#E67300"
            onClicked: root.backToMenuClicked()
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: root.currentPlayer === 1 ? "黑棋回合" : "白棋回合"
            font.pixelSize: 20
            font.bold: true
            color: "white"
        }
    }

    // 棋盘区域
    Item {
        id: boardArea
        anchors.centerIn: parent
        width: 450
        height: 450

        Image { anchors.fill: parent; source: "qrc:/res/images/board_bg.png"; fillMode: Image.PreserveAspectFit }
        Item { id: chessContainer; anchors.fill: parent }  // 棋子容器
    }

    property int currentPlayer: 1  // 当前回合玩家

    // 动态添加棋子的函数
    function addChess(row, col, type) {
        var chess = chessPieceComponent.createObject(chessContainer, {
            "row": row, "col": col, "pieceType": type, "x": col * 50, "y": row * 50
        });
        chess.drop();  // 触发落子动画
    }

    Component { id: chessPieceComponent; ChessPiece {} }  // 棋子组件引用

    // 供外部连接的信号
    signal backToMenuClicked()
    signal gameOver(bool isWin)
}
