import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 2.15

import LernappQuick

Window {
    id: window
    // width: Screen.width
    // height: Screen.height
    width: Screen.width * 0.3
    height: Screen.height * 0.6
    visible: true
    color: "#39832a"
    title: qsTr("Hello World")

    NetwortBackend {
        id: backend
    }

    StackLayout {
        id: stackLayout_1
        anchors.fill: parent
        currentIndex: 0

        ColumnLayout {
            id: columnLayout_1
            spacing: 100
            anchors.centerIn: parent
            Label {
                text: qsTr("Lernapp")
                font.pointSize: 32
                font.bold: true
                color: "#000000"
            }
            Button {
                id: buttonHomepage_start
                text: qsTr("Start")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: stackLayout_1.currentIndex = 1 | backend.refreshServer()
            }
            Button {
                id: buttonHomepage_closeApp
                text: qsTr("Beenden")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: Qt.quit()
            }
        }

        ColumnLayout {
            id: columnLayout_2
            Layout.fillHeight: true
            Layout.fillWidth: true

            Text {
                id: textFileSelected
                text: backend.nameDatabaseSelected

                font.pixelSize: 24
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            ListView {
                id: listView
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.rightMargin: 50
                Layout.leftMargin: 50
                model: backend.dataFileFromFtpServer

                delegate: ItemDelegate {
                    id: itemListView
                    width: ListView.view.width
                    required property string modelData
                    height: 40

                    Rectangle {
                        anchors.fill: parent
                        color: "#2d731e"
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
            Button {
                id: button

                text: qsTr("List Files")
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

                onClicked: backend.refreshServer()
            }
        }
    }
}
