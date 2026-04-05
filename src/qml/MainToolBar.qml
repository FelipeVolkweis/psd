import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ToolBar {
    id: toolbar
    
    FileDialog {
        id: openDialog
        title: "Escolha uma imagem"
        nameFilters: ["Image Files (*.png *.jpg *.jpeg *.bmp)"]
        fileMode: FileDialog.OpenFile
        onAccepted: imageController.loadImage(selectedFile)
    }

    FileDialog {
        id: saveDialog
        title: "Salvar Imagem"
        nameFilters: ["Image Files (*.png *.jpg)"]
        fileMode: FileDialog.SaveFile
        onAccepted: imageController.saveImage(selectedFile)
    }

    RowLayout {
        anchors.fill: parent
        Button { text: "Abrir"; onClicked: openDialog.open() }
        Button { text: "Salvar"; onClicked: saveDialog.open() }
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
            text: "Atualizar Tamanho"
            onClicked: imageController.setCanvasSize(wInput.value, hInput.value)
        }
        Button {
            text: "Croppar Imagem"
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