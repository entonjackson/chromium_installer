import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4

import org.example 1.0

Item {
    Button {
        id: button
        text: "Install latest Chromium"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        onClicked: appHandler.downloadLatest()
    }

    ProgressBar {
        id: progressBar
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: button.bottom
        anchors.margins: 10
        minimumValue: 0
        maximumValue: 1
        value: 0
        style: ProgressBarStyle {
            background: Rectangle {
                radius: 2
                color: "lightgray"
                border.color: "black"
                border.width: 2
                implicitWidth: button.width-7
                implicitHeight: 10
            }
            progress: Rectangle {
                color: "#00CC00" // green
                border.color: "black"
            }
        }
    }


    Connections {
        target: appHandler
        onUpdateProgress: {
            progressBar.value = progress
        }
        onEnableDownloadButton: {
            button.enabled = enable
        }
    }


    AppHandler {
        id: appHandler
    }
}

