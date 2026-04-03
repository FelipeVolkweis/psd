import QtQuick

Rectangle {
    id: handle
    width: 16
    height: 16
    color: "white"
    border.color: "black"
    
    property bool isLeft: false
    property bool isTop: false
    property var cursorShape: Qt.ArrowCursor
    
    // Position the handle based on properties
    anchors.left: isLeft ? parent.left : undefined
    anchors.right: !isLeft ? parent.right : undefined
    anchors.top: isTop ? parent.top : undefined
    anchors.bottom: !isTop ? parent.bottom : undefined
    anchors.margins: -8

    MouseArea {
        anchors.fill: parent
        cursorShape: handle.cursorShape
        property real startMouseX
        property real startMouseY

        onPressed: (mouse) => {
            startMouseX = mouse.x
            startMouseY = mouse.y
        }
        onPositionChanged: (mouse) => {
            let dx = mouse.x - startMouseX
            let dy = mouse.y - startMouseY
            
            if (handle.isLeft) {
                let newW = Math.max(10, mainImage.width - dx)
                let actualDx = mainImage.width - newW
                mainImage.width = newW
                mainImage.previewDx += actualDx
            } else {
                mainImage.width = Math.max(10, mainImage.width + dx)
            }
            
            if (handle.isTop) {
                let newH = Math.max(10, mainImage.height - dy)
                let actualDy = mainImage.height - newH
                mainImage.height = newH
                mainImage.previewDy += actualDy
            } else {
                mainImage.height = Math.max(10, mainImage.height + dy)
            }
        }
        onReleased: {
            let sx = mainImage.width / imageController.imageWidth
            let sy = mainImage.height / imageController.imageHeight
            if (sx !== 1.0 || sy !== 1.0 || mainImage.previewDx !== 0 || mainImage.previewDy !== 0) {
                let cx = handle.isLeft ? imageController.imageX + imageController.imageWidth : imageController.imageX
                let cy = handle.isTop ? imageController.imageY + imageController.imageHeight : imageController.imageY
                imageController.applyScale(sx, sy, cx, cy)
                mainImage.width = imageController.imageWidth
                mainImage.height = imageController.imageHeight
                mainImage.previewDx = 0
                mainImage.previewDy = 0
            }
        }
    }
}