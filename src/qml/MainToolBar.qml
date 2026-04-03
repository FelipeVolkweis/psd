import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ToolBar {
    id: toolbar
    
    FileDialog {
        id: openDialog
        title: "Please choose an image file"
        nameFilters: ["Image Files (*.png *.jpg *.jpeg *.bmp)"]
        fileMode: FileDialog.OpenFile
        onAccepted: imageController.loadImage(selectedFile)
    }

    FileDialog {
        id: saveDialog
        title: "Save Canvas"
        nameFilters: ["Image Files (*.png *.jpg)"]
        fileMode: FileDialog.SaveFile
        onAccepted: imageController.saveImage(selectedFile)
    }

    RowLayout {
        anchors.fill: parent
        Button { text: "Load"; onClicked: openDialog.open() }
        Button { text: "Save"; onClicked: saveDialog.open() }
        Item { Layout.fillWidth: true }
        Label { text: "Canvas W:" }
        SpinBox { 
            id: wInput; 
            value: imageController.hasImage ? imageController.canvasWidth : 0; 
            to: 9999; 
            editable: true 
        }
        Label { text: "Canvas H:" }
        SpinBox { 
            id: hInput; 
            value: imageController.hasImage ? imageController.canvasHeight : 0; 
            to: 9999; 
            editable: true 
        }
        Button {
            text: "Update Size"
            onClicked: imageController.setCanvasSize(wInput.value, hInput.value)
        }
        Button {
            text: "Fit to Image"
            onClicked: imageController.fitCanvasToImage()
        }
    }
    
    Connections {
        target: imageController
        function onCanvasSizeChanged() {
            wInput.value = imageController.canvasWidth
            hInput.value = imageController.canvasHeight
        }
    }
}