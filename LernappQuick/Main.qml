pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts 2.15

import LernappQuick

ApplicationWindow {
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

    // MenuBar {
    //     id: menuBar
    //     anchors.fill: parent.width
    //     Menu {
    //         title: "1"
    //     }
    //     Menu {
    //         title: "&2"
    //     }
    //     Menu {
    //         title: "&3"
    //     }
    // }

    // ToolBar {
    //     anchors.top: menuBar.bottom
    //     RowLayout {
    //         ToolButton {
    //              text: qsTr("<")
    //              onClicked: stackView_1.pop()
    //         }
    //         ToolButton {
    //              text: qsTr("⋮")
    //              onClicked: menu.open()
    //         }
    //     }
    // }

    // TabBar {
    //     width: parent.width
    //     anchors.bottom: parent.bottom
    //     TabButton {
    //         text: "1"
    //     }
    // }

    StackView {
        id: stackView_1
        anchors.fill: parent
        initialItem: columnLayout_1
    }


    ColumnLayout { // Seite 1
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

    ColumnLayout { // Seite 2
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

        ListView { // Server - Liste Datei FTP
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
                    border.color: "black"
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
                        duration: 250
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
                onClicked: backend_Network.downloadFile() | backend_Database.showLocalDatabase()
            }

            Button {
                id: button_refreshFileListServer

                text: qsTr("Liste Aktualisieren")

                onClicked: backend_Network.refreshServer()
            }
        }

        ListView { // Local - Datei Liste mit Datenbanken vom Server gespeichert
            id: listView_LocalDir

            Layout.preferredHeight: 150
            Layout.fillWidth: true
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            // Layout.bottomMargin: 25
            // Layout.topMargin: 25
            model: backend_Database.listLocalDir



            delegate: ItemDelegate {
                id: itemListView_localDir
                required property string modelData

                width: ListView.view.width

                Rectangle {
                    anchors.fill: parent
                    color: "yellow"
                    border.color: "Black"
                    border.width: 1

                    Text {
                        text: itemListView_localDir.modelData
                        color: "black"
                        font.pointSize: 16
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: backend_Database.getFileName(itemListView_localDir.modelData) | backend_Database.selectedLocalFileName << itemListView_localDir.modelData
                    }
                }
            }
            populate: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 250
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
                text: qsTr("Liste Aktualisieren")
                onClicked: backend_Database.showLocalDatabase()
            }
            Button {
                id: button_deleteFileListLocal
                text: qsTr("Datei Löschen")
                onClicked: backend_Database.deleteLocalFile(backend_Database.selectedLocalFileName) | backend_Database.showLocalDatabase()
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

    ColumnLayout { // Seite 3
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
