import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

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
                onTriggered: folderDialog.open()
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    FolderDialog {
        id: folderDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0]
        onAccepted: {
            const folder = folderDialog.selectedFolder
            player.addFolderToQueue(folder)
        }
    }

    GridLayout {
        id: grid
        columns: 2
        rows: 5
        anchors.fill: parent

        RowLayout {
            Button {
                id: playButton
                text: {player.playbackState === 1 ? qsTr("Stop") : qsTr("Play")}
                onClicked: {
                        player.playbackState === 1 ? player.pause() : player.play()
                }
            }

            Button {
                text: "Next"
                onClicked: player.next()
            }
        }

        Text {
            id: timeElapsed
            text: {
                const elapsedMs = player.position
                const elapsedMinutes = Math.floor(elapsedMs / 60000)
                const elapsedSeconds = Math.floor((elapsedMs % 60000) / 1000)
                return elapsedMinutes + ":" + (elapsedSeconds < 10 ? "0" : "") + elapsedSeconds
            }
        }

        Text {
            id: songDuration
            text: {
                const durationMs = player.duration
                const minutes = Math.floor(durationMs / 60000)
                const seconds = Math.floor((durationMs % 60000) / 1000)
                return minutes + ":" + (seconds < 10 ? "0" : "") + seconds
            }
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
        }

        // ListView {
        //     model: queueModel
        //     anchors.fill: parent
        //
        //     Layout.columnSpan: 2
        //     Layout.rowSpan: 2
        //     height: 200
        //
        //     delegate: Item {
        //         width: parent.width
        //         height: 50
        //
        //         Row {
        //             spacing: 2
        //             anchors.centerIn: parent
        //
        //             Text {
        //                 text: model.filename
        //                 width: parent.width * 0.6
        //                 elide: Text.ElideRight
        //             }
        //
        //             Button {
        //                 text: model.filename
        //                 onClicked: {
        //                     console.log("Song URL:", model.url)
        //                     player.setMediaSource(model.url)
        //                 }
        //             }
        //         }
        //     }
        // }
    }
}