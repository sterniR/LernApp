import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 2.15
import LernappQuick

Window {
    id: window
    width: 480 * 0.9
    height: 640 * 0.9
    visible: true
    color: "#2d731e"
    title: qsTr("Hello World")

    NetwortBackend {
        id: backend
    }

    Button {
        id: button
        y: 509
        text: qsTr("Refresh Server")
        anchors.horizontalCenter: parent.horizontalCenter
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        onClicked: backend.refreshServer()
    }

    Text {
        id: textFileSelected
        text: backend.dataFileFromFtpServer
        anchors.bottom: listView.top
        anchors.bottomMargin: 5
        font.pixelSize: 12
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.leftMargin: 74
        anchors.rightMargin: 74
        anchors.topMargin: 73
        anchors.bottomMargin: 73
        model: backend.dataFileFromFtpServer

        delegate: Item {
            id: itemListView
            required property string modelData
            width: ListView.view.width
            height: 40
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "white"
                border.width: 1

                Text {
                    text: itemListView.modelData
                    color: "white"
                    font.pixelSize: 16
                }

                MouseArea {
                    id: mouseAreaRefreshServer
                    anchors.fill: parent
                    onClicked: backend.ThemeDatabaseSelected(
                                   itemListView.modelData)
                }
            }
        }
    }
}
