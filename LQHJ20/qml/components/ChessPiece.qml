import QtQuick 2.15

Rectangle {
    id: chessPiece
    // 外部可设置的属性
    property int pieceType: 0  // 0=空，1=黑棋，2=白棋
    property int row: 0        // 棋子所在行
    property int col: 0        // 棋子所在列
    width: 40
    height: 40
    radius: 20  // 圆形棋子
    border.width: 2
    border.color: "#333333"

    // 根据类型设置棋子颜色
    color: pieceType === 1 ? "black" : pieceType === 2 ? "white" : "transparent"

    // 落子动画：从上方掉落并带弹跳效果
    NumberAnimation on y {
        id: dropAnimation
        from: chessPiece.y - 100  // 从上方100像素开始
        to: chessPiece.y          // 落到目标位置
        duration: 300             // 动画时长0.3秒
        easing.type: Easing.OutBounce  // 弹跳效果更生动
        running: false  // 默认不运行，需要时手动启动
    }

    function drop() {
        dropAnimation.running = true;  // 供外部调用的落子函数
    }
}
