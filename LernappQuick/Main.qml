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
    color: "#39832a"
    title: qsTr("Hello World")

    NetwortBackend {
        id: backend
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
                           columnLayout_2) //| backend.refreshServer()
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
        spacing: 10

        Label {
            id: textFileSelected
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10

            text: backend.nameDatabaseSelected
            color: "black"
            font.pixelSize: 24
        }

        ListView {
            id: listView
            Layout.preferredHeight: 500
            Layout.fillWidth: true
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            model: backend.dataFileFromFtpServer

            delegate: ItemDelegate {
                id: itemListView
                required property string modelData
                width: ListView.view.width

                Rectangle {
                    anchors.fill: parent
                    color: "#2d731e"
                    border.color: "white"
                    border.width: 1
                    Text {
                        text: itemListView.modelData
                        color: "#000000"
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
                id: button_1_s1

                text: qsTr("List Files")

                onClicked: backend.refreshServer()
            }

            Button {
                id: button_next_s1

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
            color: "blue"
            border.color: "black"
            border.width: 10
            Layout.fillWidth: parent
            Layout.fillHeight: parent
            Label {
                anchors.centerIn: parent
                text: backend.nameDatabaseSelected
                font.pointSize: 25
                color: "yellow"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: stackView_1.pop()
            }
        }
    }
}
