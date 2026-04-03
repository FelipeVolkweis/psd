import QtQuick

Rectangle {
    id: rotateHandle
    width: 16
    height: 16
    color: "cyan"
    border.color: "black"
    radius: 8
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.topMargin: -30

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        property real startRot: 0
        property real startMouseX
        property real startMouseY

        onPressed: (mouse) => {
            startRot = mainImage.rotation
            let pt = mapToItem(canvasContainer, mouse.x, mouse.y)
            startMouseX = pt.x
            startMouseY = pt.y
        }
        onPositionChanged: (mouse) => {
            let pt = mapToItem(canvasContainer, mouse.x, mouse.y)
            let cx = mainImage.x + mainImage.width / 2
            let cy = mainImage.y + mainImage.height / 2
            
            let angle1 = Math.atan2(startMouseY - cy, startMouseX - cx)
            let angle2 = Math.atan2(pt.y - cy, pt.x - cx)
            
            let diff = (angle2 - angle1) * 180 / Math.PI
            mainImage.rotation = startRot + diff
        }
        onReleased: {
            if (mainImage.rotation !== 0) {
                let cx = imageController.imageX + imageController.imageWidth / 2.0
                let cy = imageController.imageY + imageController.imageHeight / 2.0
                imageController.applyRotation(mainImage.rotation, cx, cy)
                mainImage.rotation = 0
            }
        }
    }
}