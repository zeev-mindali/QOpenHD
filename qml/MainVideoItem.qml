import QtQuick 2.12
import QtGraphicalEffects 1.12

import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12


import org.freedesktop.gstreamer.GLVideoItem 1.0;

GstGLVideoItem {
    anchors.fill: parent
    id: mainVideoItem
    objectName: "mainVideoItem"

    Text {
        id: watermark
        z: 2.0
        color: "#89ffffff"
        visible: !settings.hide_watermark
        font.pixelSize: 18
        text: "Do not fly with this app! Video is not stable yet!"
        horizontalAlignment: Text.AlignHCenter
        height: 24
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 96
    }

    Glow {
        anchors.fill: watermark
        visible: !settings.hide_watermark
        radius: 3
        samples: 17
        color: "black"
        source: watermark
    }

    MouseArea {
        id: videoMouseArea

        anchors.fill: parent
        enabled: true

        onClicked: {
            console.log("click");
            if (videoControls.visible) {
                videoControls.close();
            } else {
                videoControls.open();
            }
        }

        onPressAndHold: {
            console.log("click");

            if (videoControls.visible) {
                videoControls.close();
            } else {
                videoControls.open();
            }
        }
    }

    Popup {
        id: videoControls

        width: 240
        height: 304

        background: Rectangle {
            color: "grey"
        }

        /*
         * This centers the popup on the screen rather than positioning it
         * relative to the parent item
         *
         */
        parent: Overlay.overlay
        x: 12
        y: 64


        leftPadding: 6
        rightPadding: 6
        topPadding: 6
        bottomPadding: 0



        Item {
            anchors.fill: parent
            clip: true

            ScrollView {
                clip: true
                padding: 0
                ScrollBar.vertical.policy: ScrollBar.AlwaysOff


                anchors.fill: parent
                contentWidth: parent.width

                contentHeight: 304

                SpinBox {
                    id: brightness
                    height: 40
                    width: parent.width
                    from: -100
                    to: 100
                    stepSize: 1

                    font.pixelSize: 14
                    anchors.top: parent.top
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                CheckBox {
                    id: flushBuffers
                    height: 24
                    width: parent.width

                    font.bold: true
                    leftPadding: 0
                    anchors.top: brightness.bottom
                    text: "Flush buffers"
                    // @disable-check M223
                    Component.onCompleted: {

                    }
                    // @disable-check M222
                    onCheckedChanged: {}


                    contentItem: Text {
                        text: flushBuffers.text
                        font: flushBuffers.font
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: flushBuffers.indicator.width + flushBuffers.spacing
                    }

                    indicator: Rectangle {
                        implicitWidth: 20
                        implicitHeight: 20
                        x: flushBuffers.leftPadding
                        y: parent.height / 2 - height / 2
                        radius: 3
                        color: "#00000000"
                        border.color: "white"

                        Rectangle {
                            width: 14
                            height: 14
                            x: 3
                            y: 3
                            radius: 2
                            color: "white"
                            visible: flushBuffers.checked
                        }
                    }
                }




                ComboBox {
                    id: exposureMode
                    height: 40
                    width: parent.width

                    model: ["off","auto","night","nightpreview","backlight","spotlight","sports","snow","beach","verylong","fixedfps","antishake","fireworks"]

                    font.pixelSize: 14
                    anchors.top: flushBuffers.bottom
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.right: parent.right
                }


                ComboBox {
                    id: meteringMode
                    height: 40
                    width: parent.width

                    model: ["average","spot","backlit","matrix"]

                    font.pixelSize: 14
                    anchors.top: exposureMode.bottom
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.right: parent.right
                }


                ComboBox {
                    id: awbMode
                    height: 40
                    width: parent.width

                    model: ["off","auto","sun","cloud","shade","tungsten","fluorescent","incandescent","flash","horizon","greyworld"]

                    font.pixelSize: 14
                    anchors.top: meteringMode.bottom
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                ComboBox {
                    id: drcMode
                    height: 40
                    width: parent.width

                    model: ["off","low","med","high"]

                    font.pixelSize: 14
                    anchors.top: awbMode.bottom
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                ComboBox {
                    id: intraRefreshMode
                    height: 40
                    width: parent.width

                    model: ["cyclic","adaptive","both","cyclicrows"]

                    font.pixelSize: 14
                    anchors.top: drcMode.bottom
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                ComboBox {
                    id: profileMode
                    height: 40
                    width: parent.width

                    model: ["baseline","main","high"]

                    font.pixelSize: 14
                    anchors.top: intraRefreshMode.bottom
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }
        }
    }
}


