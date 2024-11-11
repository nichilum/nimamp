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
        id: timeElapsed
        text: {
            const elapsedMs = player.position
            const elapsedMinutes = Math.floor(elapsedMs / 60000)
            const elapsedSeconds = Math.floor((elapsedMs % 60000) / 1000)
            return elapsedMinutes + ":" + (elapsedSeconds < 10 ? "0" : "") + elapsedSeconds
        }
        anchors.top: playButton.bottom
    }

    Text {
        id: songDuration
        text: {
            const durationMs = player.duration
            const minutes = Math.floor(durationMs / 60000)
            const seconds = Math.floor((durationMs % 60000) / 1000)
            return minutes + ":" + (seconds < 10 ? "0" : "") + seconds
        }
        anchors.top: timeElapsed.bottom
    }

    Slider {
        id: seekSlider
        to: player.duration
        value: player.position
        onValueChanged: {
            if (seekSlider.pressed){
                player.position = seekSlider.value
            }
        }
        anchors.top: songDuration.bottom
    }

    Slider {
        id: volumeSlider
        to: 1
        value: player.getVolume()
        onValueChanged: {
            if (volumeSlider.pressed){
                player.setVolume(volumeSlider.value)
            }
        }
        orientation: Qt.Vertical
        anchors.top: seekSlider.bottom
    }

    ListView {
        anchors.fill: parent
        model: queueModel

        delegate: Item {
            width: parent.width
            height: 50

            Row {
                spacing: 2
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: model.filename
                    width: parent.width * 0.6
                    elide: Text.ElideRight
                }

                Button {
                    text: model.filename
                    onClicked: {
                        console.log("Song URL:", model.url)
                        player.setMediaSource(model.url)
                    }
                }
            }
        }
    }

}
