import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

import lampclient 1.0

Window {
    id: window
    visible: true
    width: 800
    height: 600

    Rectangle {
        id: backgound
        color: 'white'
        anchors.fill: parent
    }

    Rectangle {
        id: lamp
        width: 300
        height: 300
        anchors.centerIn: parent
        visible: false
        color: "yellow"
        radius: 150
        border.width: 1
        border.color: 'black'

        Text {
            anchors.centerIn: parent
            text: "Lamp"
        }

        Behavior on color {
            ColorAnimation { duration: 500 }
        }
    }

    LampClient {
        id: socket

        onLampOn: {
            lamp.visible = flag
        }
        onLampChangeColor: {
            lamp.color = color
        }
        onDisconnected: {
            fade.visible = true
            dialog.visible = true
        }
        onSocketError: {
            infoPanel.error(txt)
        }
    }

    Rectangle {
        id: fade
        anchors.fill: parent
        visible: true
        color: "#BB000000"
    }

    HostDialog {
        id: dialog
        anchors.centerIn: parent
        visible: true

        onRejected: {
            window.close()
        }
        onAccepted: {
            if (socket.connectTo(dialog.host, dialog.port)) {
                dialog.visible = false
                fade.visible = false
                infoPanel.info("Connection established: " + dialog.host + ":" + dialog.port)
            }
        }
    }

    Rectangle {
        id: infoPanel
        width: parent.width - 10
        height: 64
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 20
        border.width: 1
        border.color: 'black'

        visible: false

        property alias infoTxt: info.text

        signal error(string str)
        signal info(string str)

        onError: {
            infoTxt = "Error: " + str
            state = "BAD"
            visible = true
        }
        onInfo: {
            infoTxt = str
            state = "GOOD"
            visible = true
        }

        state: "GOOD"
        states: [
            State {
                name: "GOOD"
                PropertyChanges {
                    target: infoPanel
                    color: "#AA00FF00"
                }
            },
            State {
                name: "BAD"
                PropertyChanges {
                    target: infoPanel
                    color: "#AAFF0000"
                }
            }
        ]

        Text {
            id: info
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            text: ""
        }

        MouseArea {
            anchors.fill: parent
            onClicked:
                infoPanel.visible = false
        }
    }
}

