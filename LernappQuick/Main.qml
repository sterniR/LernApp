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
    color: "#2a82a4"


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
                color: "#2a82a4"
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

            background: Rectangle {
                color: "#2a82a4"
            }
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
                                           backend_Database.fillQuestionList() |
                                           backend_Database.fillStatusList() |
                                           backend_Database.fillOptionList() |
                                           backend_Database.fillTrueFalseList() |
                                           backend_Database.fillInputList()
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

            background: Rectangle {
                color: "#2a82a4"
            }
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
                    text: "Thema: " + backend_Database.selectedLocalFileName
                    font.pointSize: 25
                    color: "black"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter

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
                        onClicked: {
                            stackView_1.push(pageFragenBearbeiten);
                            backend_Database.fillStatusList;
                            backend_Database.nextWord();
                            Qt.callLater(() => {
                                             if (backend_Database.statusList.length > 0) {
                                                 stackLayout_1.currentIndex = backend_Database.statusList[0] - 1;
                                             }
                                         });
                        }
                    }
                }
            }
        }

        Page { // Seite 4
            id: pageFragenBearbeiten
            visible: false
            property var optionList: backend_Database.optionListFromMultiQuestion[stackLayout_1.index]
            Layout.fillHeight: true
            Layout.fillWidth: true

            onVisibleChanged: {
                if (visible && backend_Database.statusList.length > 0) {
                    stackLayout_1.currentIndex = backend_Database.statusList[0];
                }
            }

            header: ToolBar {
                background: Rectangle {
                    color: "#2a82a4"
                }
                RowLayout {
                    anchors.fill: parent
                    Text {
                        Layout.alignment: Qt.AlignRight

                        text: backend_Database.currentIndex + " / " + backend_Database.statusList.length
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
                property int index: 0

                ColumnLayout { // True oder False
                    id: columnLayoutTrueFalse
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    property string wert: backend_Database.userAnswerTrueFalse
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // Layout.topMargin: 50
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
                                text: backend_Database.question
                                color: "black"
                                font.pixelSize: 24
                            }
                        }

                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                            Button {
                                id: buttonTrueFalse1
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                Layout.fillWidth: true
                                implicitHeight: 50

                                Text {
                                    anchors.centerIn: parent
                                    text: "Richtig"
                                    color: "black"
                                    font.pixelSize: 24
                                }

                                background: Rectangle {

                                    color: "white"
                                    border.color: buttonTrueFalse1.down ? "white" : "black"
                                    border.width: 5
                                }

                                onClicked: {
                                    columnLayoutTrueFalse.wert = "richtig";
                                }
                            }

                            Button {
                                id: buttonTrueFalse2
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                Layout.fillWidth: true
                                implicitHeight: 50

                                background: Rectangle {

                                    color: "white"
                                    border.color: buttonTrueFalse2.down ? "white" : "black"
                                    border.width: 5
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: "Falsch"
                                    color: "black"
                                    font.pixelSize: 24
                                }

                                onClicked: {
                                    columnLayoutTrueFalse.wert = "falsch";
                                }
                            }
                        }
                        Button {
                            id: buttonWeiter1
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.fillWidth: true
                            implicitHeight: 50

                            background: Rectangle {

                                color: "white"
                                border.color: buttonWeiter1.down ? "white" : "black"
                                border.width: 5
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Weiter →"
                                color: "black"
                                font.pixelSize: 24
                            }
                            onClicked: {
                                backend_Database.nextWord();
                                backend_Database.checkAnswerTrueFalse(columnLayoutTrueFalse.wert);
                                // backend_Database.userAnswerTrueFalseChanged();
                                stackLayout_1.index++;

                                if (stackLayout_1.index >= backend_Database.statusList.length) {
                                    stackView_1.push(pageReward)
                                }
                                stackLayout_1.currentIndex = backend_Database.statusList[stackLayout_1.index] - 1;
                            }
                        }
                    }
                }


                ColumnLayout { // 3 x Multiple Choise
                    id: columnLayout3x
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    property string wert: backend_Database.userAnswerMultiChoise
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // Layout.topMargin: 50
                        Layout.bottomMargin: 50
                        Layout.leftMargin: 50
                        Layout.rightMargin: 50

                        ButtonGroup {
                            id: buttonGroupRadioButton
                        }

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
                                text: backend_Database.question
                                color: "black"
                                font.pixelSize: 24
                            }
                        }

                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            model: pageFragenBearbeiten.optionList
                            spacing: 20
                            interactive: false

                            delegate: ItemDelegate {
                                id: item
                                width: ListView.view.width
                                height: 50
                                required property string modelData
                                ColumnLayout {
                                    width: parent.width
                                    RowLayout {
                                        Layout.fillWidth: true

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
                                                onCheckedChanged: columnLayout3x.wert = item.modelData

                                            }
                                        }
                                    }
                                }

                            }
                        }
                        Button {
                            id: buttonWeiter2
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.fillWidth: true
                            implicitHeight: 50

                            background: Rectangle {

                                color: "white"
                                border.color: buttonWeiter2.down ? "white" : "black"
                                border.width: 5
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Weiter →"
                                color: "black"
                                font.pixelSize: 24
                            }
                            onClicked: {
                                backend_Database.nextWord();
                                backend_Database.checkAnswerMultiChoise(columnLayout3x.wert);
                                // backend_Database.userAnswerMultiChoiseChanged();
                                stackLayout_1.index++;

                                if (stackLayout_1.index >= backend_Database.statusList.length) {
                                    stackView_1.push(pageReward)
                                }
                                stackLayout_1.currentIndex = backend_Database.statusList[stackLayout_1.index] - 1;
                            }
                        }

                    }
                }


                ColumnLayout { // Ein Wort Input
                    id: columnLayoutOneWord
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    property string wert: backend_Database.userAnswerInput
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // Layout.topMargin: 50
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
                                text: backend_Database.question
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
                                    text: "_"
                                    font.pixelSize: 24
                                    onTextChanged: columnLayoutOneWord.wert = text
                                }
                            }
                        }
                        Button {
                            id: buttonWeiter3
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.fillWidth: true
                            implicitHeight: 50

                            background: Rectangle {

                                color: "white"
                                border.color: buttonWeiter3.down ? "white" : "black"
                                border.width: 5
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Weiter →"
                                color: "black"
                                font.pixelSize: 24
                            }
                            onClicked: {
                                backend_Database.nextWord();
                                backend_Database.checkAnswerInput(columnLayoutOneWord.wert);
                                stackLayout_1.index++;

                                if (stackLayout_1.index >= backend_Database.statusList.length) {
                                    stackView_1.push(pageReward)
                                }
                                stackLayout_1.currentIndex = backend_Database.statusList[stackLayout_1.index] - 1;
                            }
                        }
                    }
                }
            }
        }            


        Page { // Seite 5
            id: pageReward
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: false
            background: Rectangle {
                color: "#2a82a4"
            }
            ColumnLayout {
                anchors.centerIn: parent
                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: "🏁 Du hast \n" + backend_Database.correctPoints + "/" + backend_Database.questionList.length + "\nrichtig beantwortet.\nSehr gut!"
                    color: "black"
                    font.pixelSize: 24

                }
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    implicitHeight: 50
                    // Button {
                    //     Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    //     text: "Wiederholen"

                    // }
                    Button {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: "Startseite"
                        onClicked: {
                            backend_Database.counterQuestion = 0;
                            backend_Database.currentIndex = 0;
                            backend_Database.setCorrectPoints(0);
                            stackLayout_1.index = 0;
                            stackView_1.popToIndex(1);
                        }

                    }
                }

            }

        }



    }


    footer: TabBar {

    }
}
