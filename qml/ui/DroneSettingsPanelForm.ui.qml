import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Qt.labs.settings 1.0

import OpenHD 1.0

Item {
    property alias save: save
    property alias showSavedCheckmark: savedCheckmark.visible

    Layout.fillHeight: true
    Layout.fillWidth: true

    TabBar {
        id: settingsBar
        width: parent.width
        height: 48
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top


        TabButton {
            y: 0
            text: qsTr("General")
            width: implicitWidth
            height: 48
            font.pixelSize: 13
        }

        TabButton {
            y: 0
            text: qsTr("Ports")
            width: implicitWidth
            height: 48
            font.pixelSize: 13
        }

        TabButton {
            y: 0
            text: qsTr("RC")
            width: implicitWidth
            height: 48
            font.pixelSize: 13
        }

        TabButton {
            text: qsTr("Other")
            width: implicitWidth
            height: 48
            font.pixelSize: 13
        }
    }

    StackLayout {
        id: droneSettings
        anchors.top: settingsBar.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: button_background.top
        anchors.bottomMargin: 0


        currentIndex: settingsBar.currentIndex

        DroneSettingsListView {
            id: generalTab
            width: parent.width
            height: parent.height
            model: generalParametersModel
        }

        DroneSettingsListView {
            id: portsTab
            model: portParametersModel
            width: parent.width
            height: parent.height
        }

        DroneSettingsListView {
            id: rcTab
            model: rcParametersModel
            width: parent.width
            height: parent.height
        }

        DroneSettingsListView {
            id: otherTab
            model: otherParametersModel
            width: parent.width
            height: parent.height
        }
    }


    Rectangle {
        color: "#4c000000"
        width: parent.width
        height: 1

        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: button_background.top
        anchors.bottomMargin: 0
    }

    Rectangle {
        id: button_background
        width: parent.width
        height: 64
        color: "#fafafa"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        RowLayout {
            id: button_row
            height: parent.height
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            anchors.rightMargin: 24
            anchors.leftMargin: 12

            Item {
                id: spacer
                Layout.fillWidth: true
                Layout.rowSpan: 1
                Layout.columnSpan: 1
                Layout.preferredHeight: 14
                Layout.preferredWidth: 14
            }

            Button {
                id: save
                text: qsTr("Save")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                enabled: !mavlinkTelemetry.loading && !mavlinkTelemetry.saving
                Layout.columnSpan: 1
                font.pixelSize: 13
            }
        }

        BusyIndicator {
            id: busyIndicator
            y: 0
            width: 36
            height: 36
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: button_row.left
            anchors.rightMargin: 0
            running: mavlinkTelemetry.loading || mavlinkTelemetry.saving
            visible: mavlinkTelemetry.loading || mavlinkTelemetry.saving
        }

        Item {
            id: savedCheckmark
            visible: false

            anchors.verticalCenter: parent.verticalCenter
            anchors.right: button_row.left
            anchors.rightMargin: 0

            Text {
                id: savedText
                text: "saved"
                font.pixelSize: 20
                height: parent.height
                anchors.right: savedIcon.left
                anchors.rightMargin: 12
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                id: savedIcon
                text: "\uf00c"
                font.family: "Font Awesome 5 Free"
                font.pixelSize: 20
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: 0
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
