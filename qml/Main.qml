import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    // Button {
    //     text: "aa"
    //     onClicked: player.print("aa")
    // }

    Button {
        id: playButton
        text: {player.playbackState === 1 ? qsTr("Stop") : qsTr("Play")}
        onClicked: {
            player.playbackState === 1 ? player.pause() : player.play()
        }
    }

    Button {
        text: "Next"
        anchors.left: playButton.right
        onClicked: player.next()
    }

    Text {
        property int durationMs: player.duration
        property int minutes: Math.floor(durationMs / 60000)
        property int seconds: Math.floor((durationMs % 60000) / 1000)

        id: songTitle
        text: minutes + ":" + (seconds < 10 ? "0" : "") + seconds
        anchors.top: playButton.bottom
    }

}
