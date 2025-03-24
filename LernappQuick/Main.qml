pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts 2.15

import LernappQuick

Window {
    id: window
    // width: Screen.width
    // height: Screen.height
    width: Screen.width * 0.3
    height: Screen.height * 0.6
    minimumWidth: 420
    minimumHeight: 680
    visible: true
    color: "white"
    title: qsTr("Hello World")

    Network{
        id: backend_Network
    }
    Database {
        id: backend_Database
    }

    StackView {
        id: stackView_1
        anchors.fill: parent
        initialItem: columnLayout_1
    }

    ColumnLayout {
        id: columnLayout_1

        spacing: 10
        // Layout.preferredHeight: parent.height
        // Layout.preferredWidth: parent.width
        Layout.fillHeight: true
        Layout.fillWidth: true
        Label {
            text: qsTr("Lernapp")
            font.pointSize: 32
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignHCenter
        }
        Button {
            id: buttonHomepage_start
            text: qsTr("Start")
            Layout.alignment: Qt.AlignHCenter
            onClicked: stackView_1.push(
                           columnLayout_2) | backend_Database.setupDatabaseDir()
        }
        Button {
            id: buttonHomepage_closeApp
            text: qsTr("Beenden")
            Layout.alignment: Qt.AlignHCenter
            onClicked: Qt.quit()
        }
    }

    ColumnLayout {
        id: columnLayout_2
        Layout.fillHeight: true
        Layout.fillWidth: true
        visible: false
        spacing: 5

        Label {
            id: textFileSelected
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10

            text: backend_Network.nameDatabaseSelected
            color: "black"
            font.pixelSize: 24
        }

        ListView {
            id: listView_Server
            Layout.preferredHeight: 150
            Layout.fillWidth: true
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            // Layout.bottomMargin: 25
            // Layout.topMargin: 25
            model: backend_Network.dataFileFromFtpServer

            delegate: ItemDelegate {
                id: itemListView_Server
                width: ListView.view.width
                required property string modelData
                Rectangle {
                    anchors.fill: parent
                    color: "#2d731e"
                    border.color: "white"
                    border.width: 1
                    Text {
                        text: itemListView_Server.modelData
                        color: "#000000"
                        font.pixelSize: 16
                    }

                    MouseArea {
                        id: mouseAreaRefreshServer
                        anchors.fill: parent
                        onClicked: backend_Network.ThemeDatabaseSelected(
                                       itemListView_Server.modelData)
                    }
                }
            }
            populate: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 750
                        easing.type: Easing.InOutCubic
                        from: 0
                        to: 1
                }
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            // Layout.bottomMargin: 10

            Button {
                id: button_downloadFile
                text: qsTr("Datei Herunterladen")
                onClicked: backend_Network.downloadFile()
            }

            Button {
                id: button_refreshFileListServer

                text: qsTr("Liste Aktualisieren")

                onClicked: backend_Network.refreshServer()
            }
        }
        ListView {
            id: listView_LocalDir

            Layout.preferredHeight: 150
            Layout.fillWidth: true
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            // Layout.bottomMargin: 25
            // Layout.topMargin: 25
            model: backend_Database.listLocalDatabase()

            delegate: ItemDelegate {
                id: itemListView_localDir
                required property var modelData

                width: ListView.view.width

                Rectangle {
                    anchors.fill: parent
                    color: "yellow"
                    border.color: "Black"
                    border.width: 1
                    Text {
                        text: backend_Database.listLocalDatabase()
                        color: "black"
                        font.pointSize: 16
                    }
                }
            }
            populate: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 750
                        easing.type: Easing.InOutCubic
                        from: 0
                        to: 1
                }
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.bottomMargin: 10
            Button {
                id: button_refreshFileListLocal
                text: qsTr("Aktualisieren")
                onClicked: backend_Database.listLocalFile
            }
        }


        RowLayout {
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.bottomMargin: 10
            Button {
                id: button_nextToView3
                text: qsTr("Weiter")
                onClicked: stackView_1.push(columnLayout_3)
            }
        }

    }

    ColumnLayout {
        id: columnLayout_3
        Layout.fillHeight: true
        Layout.fillWidth: true
        visible: false

        Rectangle {
            id: rect_3
            color: "blue"
            border.color: "black"
            border.width: 10
            Layout.fillWidth: parent
            Layout.fillHeight: parent
            Label {
                anchors.centerIn: parent
                text: backend_Network.nameDatabaseSelected
                font.pointSize: 25
                color: "yellow"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: stackView_1.pop()
            }
            Text {
                text: backend_Database.setupDatabaseDir()
                font.pointSize: 8
                color: "red"
            }
        }

    }
}
