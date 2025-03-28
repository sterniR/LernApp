pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts 2.15

import LernappQuick

ApplicationWindow {
    id: window
    width: Screen.width * 0.3
    height: Screen.height * 0.6
    minimumWidth: 420
    minimumHeight: 680
    visible: true
    title: qsTr("Lernapp")
    color: "white"


    Network{
        id: backend_Network
    }
    Database {
        id: backend_Database
    }


    header: ToolBar {
        id: toolBar

        RowLayout {
            anchors.fill: parent
            ToolButton {
                Layout.alignment: Qt.AlignLeft
                text: qsTr("←")
                onClicked: stackView_1.pop()
            }
            Button {
                Layout.alignment: Qt.AlignRight
                text: qsTr("?")
                ToolTip.visible: down
                ToolTip.text: qsTr("Lernapp von Roman FIAE 22")
            }
        }
    }


    StackView {
        id: stackView_1
        anchors.fill: parent

        initialItem: Page { // Seite 1
            id: pageHome
            background: Rectangle {
                color: "white"
            }
            ColumnLayout {
                id: columnLayout_1
                spacing: 10
                anchors.fill: parent
                Label {
                    text: qsTr("Lernapp")
                    font.pointSize: 32
                    font.bold: true
                    color: "black"
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


        }


        Page { // Seite 2
            id: pageDownloadFiles
            // title: qsTr("Download")
            visible: false
            ColumnLayout {
                id: columnLayout_2
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 5

                Label {
                    id: textFileServerSelected
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 10

                    text: "Server: " + backend_Network.nameDatabaseSelected
                    color: "black"
                    font.pixelSize: 24
                }

                ListView { // Server - Liste Datei FTP
                    id: listView_Server
                    Layout.preferredHeight: 150
                    Layout.fillWidth: true
                    Layout.rightMargin: 50
                    Layout.leftMargin: 50

                    model: backend_Network.dataFileFromFtpServer

                    delegate: ItemDelegate {
                        id: itemListView_Server
                        width: ListView.view.width
                        required property string modelData
                        Rectangle {
                            anchors.fill: parent
                            color: mouseAreaRefreshServer.pressed ? "blue" : "#2d731e"
                            border.color: "black"
                            border.width: 1

                            Label {
                                anchors.centerIn: parent
                                color: "#000000"
                                font.pixelSize: 24
                                text: itemListView_Server.modelData
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

                ToolSeparator {
                    orientation: Qt.Horizontal
                    Layout.fillWidth: parent
                }

                Label {
                    id: textFileLocalSelected
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 10

                    text: "Local: " + backend_Database.selectedLocalFileName
                    color: "black"
                    font.pixelSize: 24
                }

                ListView { // Local - Datei Liste mit Datenbanken vom Server gespeichert
                    id: listView_LocalDir

                    Layout.preferredHeight: 150
                    Layout.fillWidth: true
                    Layout.rightMargin: 50
                    Layout.leftMargin: 50
                    model: backend_Database.listLocalDir



                    delegate: ItemDelegate {
                        id: itemListView_localDir
                        required property string modelData

                        width: ListView.view.width

                        Rectangle {
                            anchors.fill: parent
                            color: mouseAreaRefreshLocal.pressed ? "blue" : "yellow"
                            border.color: "Black"
                            border.width: 1

                            Label {
                                anchors.centerIn: parent
                                text: itemListView_localDir.modelData
                                color: "black"
                                font.pointSize: 24
                            }
                            MouseArea {
                                id: mouseAreaRefreshLocal
                                anchors.fill: parent
                                onClicked: backend_Database.getFileName(itemListView_localDir.modelData) |
                                           backend_Database.selectedLocalFileName << itemListView_localDir.modelData |
                                           columnLayout_3.countQuestions << backend_Database.getNumberOfQuestions(backend_Database.selectedLocalFileName)
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
                        id: button_deleteFileListLocal
                        text: qsTr("Datei Löschen")
                        onClicked: backend_Database.deleteLocalFile(backend_Database.selectedLocalFileName) | backend_Database.showLocalDatabase() | backend_Database.getFileName("")
                    }
                    Button {
                        id: button_refreshFileListLocal
                        text: qsTr("Liste Aktualisieren")
                        onClicked: backend_Database.showLocalDatabase()
                    }
                }

                ToolSeparator {
                    orientation: Qt.Horizontal
                    Layout.fillWidth: parent
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

        }


        Page { // Seite 3
            id: pageInfo
            visible: false
            anchors.fill: parent
            ColumnLayout {
                id: columnLayout_3
                Layout.fillHeight: true
                Layout.fillWidth: true

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("Informationen zum Thema")
                    font.pointSize: 25
                    color: "black"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    // horizontalAlignment: Qt.AlignRight
                    text: "Thema: " + backend_Database.selectedLocalFileName
                    font.pointSize: 25
                    color: "green"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    // horizontalAlignment: Qt.AlignRight

                    text: "Fragen: " + backend_Database.getNumberOfQuestions(backend_Database.selectedLocalFileName)
                    font.pointSize: 25
                    color: "black"
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    Layout.bottomMargin: 10

                    Button {
                        text: qsTr("Zurück")
                        onClicked: stackView_1.pop()
                    }

                    Button {
                        text: "Lernen beginnen →"
                        onClicked: stackView_1.push(pageFragenBearbeiten)
                    }

                }
            }


        }

        Page { // Seite 4
            id: pageFragenBearbeiten
            visible: false
            anchors.fill: parent

            header: ToolBar {
                background: Rectangle {
                    color: "white"
                }
                RowLayout {
                    anchors.fill: parent
                    Text {
                        Layout.alignment: Qt.AlignRight

                        text: "0 / " + backend_Database.getNumberOfQuestions(backend_Database.selectedLocalFileName)
                        font.pixelSize: 32
                    }
                }
            }
            background: Rectangle {
                color: "#2a82a4"
            }
            StackLayout {
                id: stackLayout_1
                anchors.fill: parent
                currentIndex: 0

                ColumnLayout { // True oder False
                    id: columnLayoutTrueFalse
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Layout.topMargin: 50
                        Layout.bottomMargin: 50
                        Layout.leftMargin: 50
                        Layout.rightMargin: 50

                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            text: "Frage" // Frage hier rein
                            font.pixelSize: 48
                            color: "black"
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "white"
                            border.color: "black"
                            border.width: 5
                            Text {
                                anchors.centerIn: parent
                                text: "1"
                                color: "black"
                                font.pixelSize: 24
                            }
                        }

                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Button {
                                Layout.alignment: Qt.AlignHCenter
                                text: "Richtig"
                            }
                            Button {
                                Layout.alignment: Qt.AlignHCenter
                                text: "Falsch"
                            }
                        }
                    }
                }


                ColumnLayout { // 3 x Multiple Choise
                    id: columnLayout3x
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Layout.topMargin: 50
                        Layout.bottomMargin: 50
                        Layout.leftMargin: 50
                        Layout.rightMargin: 50

                        ButtonGroup {
                            id: buttonGroupRadioButton
                        }

                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            text: "Frage" // Frage hier rein
                            font.pixelSize: 48
                            color: "black"
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "white"
                            border.color: "black"
                            border.width: 5
                            Text {
                                anchors.centerIn: parent
                                text: "2"
                                color: "black"
                                font.pixelSize: 24
                            }
                        }

                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            model: ["Option 1", "Option 2", "Option 3"] // Ihr kommen die Antworten rein
                            delegate: ItemDelegate {
                                id: item
                                width: ListView.view.width
                                height: 50
                                required property string modelData
                                ColumnLayout {
                                    width: parent.width
                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 0

                                        Rectangle {
                                            Layout.fillWidth: true
                                            implicitHeight: 50
                                            color: "white"
                                            border.color: "black"
                                            border.width: 5
                                            Text {
                                                anchors.centerIn: parent
                                                text: item.modelData
                                                color: "black"
                                                font.pixelSize: 24
                                            }
                                        }
                                        Rectangle {
                                            implicitWidth: 50
                                            implicitHeight: 50
                                            color: "white"
                                            border.color: "black"
                                            border.width: 5
                                            RadioButton {
                                                anchors.centerIn: parent
                                                ButtonGroup.group: buttonGroupRadioButton
                                            }
                                        }
                                    }
                                }

                            }
                        }

                    }
                }
                ColumnLayout { // Ein Wort Input
                    id: columnLayoutOneWord
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Layout.topMargin: 50
                        Layout.bottomMargin: 50
                        Layout.leftMargin: 50
                        Layout.rightMargin: 50

                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            text: "Frage"
                            font.pixelSize: 48
                            color: "black"
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "white"
                            border.color: "black"
                            border.width: 5
                            Text {
                                anchors.centerIn: parent
                                text: "3"
                                color: "black"
                                font.pixelSize: 24
                            }
                        }

                        RowLayout {
                            Rectangle {
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                Layout.fillWidth: true
                                implicitHeight: 50
                                color: "white"
                                border.color: "black"
                                border.width: 5
                                TextInput {
                                    anchors.centerIn: parent
                                    text: "Eingabe"
                                    font.pixelSize: 24
                                }
                            }
                        }
                    }
                }
            }

            footer: ToolBar {
                background: Rectangle {
                    color: "purple"
                }

                RowLayout {
                    anchors.fill: parent

                    Button {
                        Layout.alignment: Qt.AlignHCenter
                        text: "go"
                        onClicked: stackLayout_1.currentIndex < 2 ? stackLayout_1.currentIndex++ : stackLayout_1.currentIndex = 0
                    }
                }

            }
        }
    }

    footer: TabBar {

    }

}
