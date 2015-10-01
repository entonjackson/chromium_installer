import QtQuick 2.5
import QtQuick.Controls 1.2

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
        width: button.width
        minimumValue: 0
        maximumValue: 1
        value: 0
    }


    Connections {
        target: appHandler
        onUpdateProgress: {
            progressBar.value = progress
        }
    }


    AppHandler {
        id: appHandler
    }
}

