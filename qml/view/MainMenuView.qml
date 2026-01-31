import QtQuick
import QtQuick.Controls
//下面是参考界面
Rectangle {
    anchors.fill: parent
    color: "#2c3e50"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: "灵棋幻境"
            font.pixelSize: 48
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            text: "开始剧情"
            width: 200
            height: 60
            font.pixelSize: 24
            onClicked: {
                // 1. 加载剧情章节
                app.story.loadChapter("chapter1.json");
                // 2. 通知切换到剧情页面
                app.navigateTo("Story");
            }
        }
    }
}
