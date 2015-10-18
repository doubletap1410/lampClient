import QtQuick 2.5

Item {
    id: dialog
    width: 500
    height: 300

    property alias host: host.text
    property alias port: port.text
    signal rejected
    signal accepted

    Rectangle {
        id: background
        color: "#F0F0F0"
        anchors.fill: parent

//        border.width: 1
//        border.color: 'black'

        TextMetrics {
            id: hostMetrics;
            font.family:  "Times new roman"
            font.pixelSize: 30
            text: host.text
        }

        TextMetrics {
            id: portMetrics;
            font: hostMetrics.font
            text: "9999"
        }

        Text {
            id: info
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20

            font: hostMetrics.font
            text: "Input address: "
        }

        Rectangle {
            id: hostBackground
            width: hostMetrics.boundingRect.width + 10
            height: hostMetrics.boundingRect.height

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: info.right
            anchors.leftMargin: 5

            color: 'white'

            TextInput {
                id: host
                font: hostMetrics.font
                anchors.fill: parent
                text: "127.0.0.1"
                validator: RegExpValidator{regExp: /^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$/ }
            }
        }

        Text {
            id: spliter
            font: hostMetrics.font
            text: ":"
            anchors.verticalCenter: hostBackground.verticalCenter
            anchors.left: hostBackground.right
            anchors.leftMargin: 2
        }

        Rectangle {
            id: portBackground
            width: portMetrics.boundingRect.width + 10
            height: portMetrics.boundingRect.height

            anchors.verticalCenter: hostBackground.verticalCenter
            anchors.left: spliter.right
            anchors.leftMargin: 2

            color: 'white'
            TextInput {
                id: port
                anchors.fill: parent
                font: portMetrics.font
                text: "9999"
                validator: IntValidator{}
                inputMask: ">HHHH"
            }
        }

        Rectangle {
            id: acceptBtn
            width: 128
            height: 64

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right:  parent.right
            anchors.rightMargin: 10

            Text {
                anchors.centerIn: parent
                font: hostMetrics.font
                text: "Connect"
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                    accepted()
            }
        }

        Rectangle {
            id: rejectBtn
            width: 128
            height: 64

            anchors.bottom: acceptBtn.bottom
            anchors.right:  acceptBtn.left
            anchors.rightMargin: 10

            Text {
                anchors.centerIn: parent
                font: hostMetrics.font
                text: "Cancle"
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                    rejected()
            }
        }
    }
}

