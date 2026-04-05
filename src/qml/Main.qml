import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 768
    title: "Photoshop Gambiarra"

    header: MainToolBar {}

    MessageDialog {
        id: errorDialog
        title: "Error"
        buttons: MessageDialog.Ok
    }

    Connections {
        target: imageController
        function onErrorOccurred(message) {
            errorDialog.text = message
            errorDialog.open()
        }
    }

    property int updateCounter: 0
    property real zoomFactor: 1.0

    function centerView() {
        if (imageController.hasImage) {
            canvasScrollView.ScrollBar.horizontal.position = (canvasScrollView.ScrollBar.horizontal.size < 1.0) ? (1.0 - canvasScrollView.ScrollBar.horizontal.size) / 2 : 0
            canvasScrollView.ScrollBar.vertical.position = (canvasScrollView.ScrollBar.vertical.size < 1.0) ? (1.0 - canvasScrollView.ScrollBar.vertical.size) / 2 : 0
        }
    }

    Connections {
        target: imageController
        function onImageUpdated() { updateCounter++ }
        function onImageBoundsChanged() {
            mainImage.width = imageController.imageWidth
            mainImage.height = imageController.imageHeight
        }
        function onHasImageChanged() {
            if (imageController.hasImage) {
                root.zoomFactor = 1.0
                centerTimer.start()
            }
        }
    }

    Timer {
        id: centerTimer
        interval: 10
        onTriggered: root.centerView()
    }

    onZoomFactorChanged: root.centerView()

    Item {
        anchors.fill: parent

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Text {
                    anchors.centerIn: parent
                    text: "Sem Imagem"
                    color: "#999999"
                    font.pixelSize: 24
                    visible: !imageController.hasImage
                }

                ScrollView {
                    id: canvasScrollView
                    anchors.fill: parent
                    visible: imageController.hasImage
                    clip: true
                    ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    Rectangle {
                        id: scrollContent
                        width: Math.max((imageController.canvasWidth * root.zoomFactor) + 50, canvasScrollView.width)
                        height: Math.max((imageController.canvasHeight * root.zoomFactor) + 50, canvasScrollView.height)
                        color: "#333333"

                        // Zoom MouseArea
                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.NoButton
                            onWheel: (wheel) => {
                                if (wheel.modifiers & Qt.ControlModifier) {
                                    var zoomDelta = wheel.angleDelta.y > 0 ? 1.1 : 1 / 1.1;
                                    var newZoom = root.zoomFactor * zoomDelta;
                                    root.zoomFactor = Math.max(0.1, Math.min(newZoom, 10.0));
                                    wheel.accepted = true;
                                } else {
                                    wheel.accepted = false;
                                }
                            }
                        }

                        Item {
                            id: zoomContainer
                            anchors.centerIn: parent
                            width: imageController.canvasWidth
                            height: imageController.canvasHeight
                            scale: root.zoomFactor

                            Rectangle {
                                id: canvasContainer
                                anchors.fill: parent
                                color: "transparent"
                                border.color: "#888888"
                                border.width: 1 / root.zoomFactor

                                // 1. The darkened image (unclipped)
                                Image {
                                    id: mainImage
                                    source: imageController.hasImage ? "image://processor/current?id=" + root.updateCounter : ""
                                    width: imageController.hasImage ? imageController.imageWidth : 0
                                    height: imageController.hasImage ? imageController.imageHeight : 0
                                    fillMode: Image.Stretch
                                    cache: false
                                    asynchronous: false
                                    transformOrigin: Item.Center

                                    property real previewDx: 0
                                    property real previewDy: 0

                                    x: (imageController.hasImage ? imageController.imageX : 0) + previewDx
                                    y: (imageController.hasImage ? imageController.imageY : 0) + previewDy

                                    Rectangle {
                                        anchors.fill: parent
                                        color: "black"
                                        opacity: 0.5
                                    }
                                }

                                // 2. The bright image inside canvas (clipped)
                                Item {
                                    anchors.fill: parent
                                    clip: true

                                    Image {
                                        source: mainImage.source
                                        width: mainImage.width
                                        height: mainImage.height
                                        x: mainImage.x
                                        y: mainImage.y
                                        rotation: mainImage.rotation
                                        transformOrigin: Item.Center
                                        fillMode: Image.Stretch
                                        cache: false
                                        asynchronous: false
                                    }
                                }

                                // 3. Handles overlay
                                Item {
                                    id: handlesContainer
                                    width: mainImage.width
                                    height: mainImage.height
                                    x: mainImage.x
                                    y: mainImage.y
                                    rotation: mainImage.rotation
                                    transformOrigin: Item.Center

                                    Rectangle {
                                        anchors.fill: parent
                                        color: "transparent"
                                        border.color: "grey"
                                        border.width: 1 / root.zoomFactor
                                    }

                                    // Drag to translate
                                    MouseArea {
                                        anchors.fill: parent
                                        property real startX
                                        property real startY
                                        
                                        onPressed: (mouse) => {
                                            startX = mouse.x
                                            startY = mouse.y
                                        }
                                        onPositionChanged: (mouse) => {
                                            mainImage.previewDx += (mouse.x - startX)
                                            mainImage.previewDy += (mouse.y - startY)
                                        }
                                        onReleased: {
                                            if (mainImage.previewDx !== 0 || mainImage.previewDy !== 0) {
                                                imageController.applyTranslation(mainImage.previewDx, mainImage.previewDy)
                                                mainImage.previewDx = 0
                                                mainImage.previewDy = 0
                                            }
                                        }
                                    }

                                    // Resize Handles
                                    ResizeHandle { isLeft: false; isTop: false; cursorShape: Qt.SizeFDiagCursor } // Bottom Right
                                    ResizeHandle { isLeft: true;  isTop: true;  cursorShape: Qt.SizeFDiagCursor } // Top Left
                                    ResizeHandle { isLeft: false; isTop: true;  cursorShape: Qt.SizeBDiagCursor } // Top Right
                                    ResizeHandle { isLeft: true;  isTop: false; cursorShape: Qt.SizeBDiagCursor } // Bottom Left

                                    // Rotate Handle
                                    RotateHandle {}
                                }
                            }
                        }
                    }
                }
            }

            FilterSidebar {
                Layout.fillHeight: true
            }
        }
    }

    footer: ToolBar {
        height: 30
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            Label {
                text: "Zoom: " + Math.round(root.zoomFactor * 100) + "%"
                color: "white"
            }
            Slider {
                from: 0.1
                to: 1.5
                value: root.zoomFactor
                onMoved: root.zoomFactor = value
                Layout.preferredWidth: 200
            }
            Button {
                text: "100%"
                onClicked: root.zoomFactor = 1.0
            }
            Item { Layout.fillWidth: true }
        }
    }
}
