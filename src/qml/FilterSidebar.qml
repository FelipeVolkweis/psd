import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: sidebar
    width: 300 // Slightly wider to accommodate contrast points
    color: "#222222"
    border.color: "#444444"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Label {
            text: "Filter Pipeline"
            font.bold: true
            font.pixelSize: 18
            color: "white"
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: "Add Filter"
            Layout.fillWidth: true
            onClicked: addFilterMenu.open()

            Menu {
                id: addFilterMenu
                y: parent.height
                MenuItem { text: "Inverse"; onClicked: imageController.addInverseFilter() }
                MenuItem { text: "Gamma"; onClicked: imageController.addGammaFilter(1.0) }
                MenuItem { text: "Log"; onClicked: imageController.addLogFilter(1.0) }
                MenuItem { text: "Contrast"; onClicked: imageController.addContrastFilter(0.0) }
            }
        }

        ListView {
            id: filterListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: imageController.filters
            spacing: 5
            clip: true

            delegate: Rectangle {
                id: filterDelegate
                width: filterListView.width
                height: filterColumn.height + 15
                color: "#333333"
                border.color: "#555555"
                radius: 4

                // Capture roles to avoid shadowing in nested components
                readonly property int fIndex: index
                readonly property string fName: name
                readonly property var fPoints: model.points
                readonly property var fValue: model.value

                ColumnLayout {
                    id: filterColumn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 8
                    spacing: 4

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            text: filterDelegate.fName
                            color: "white"
                            font.bold: true
                            Layout.fillWidth: true
                        }
                        Button {
                            text: "✕"
                            flat: true
                            onClicked: imageController.removeFilter(filterDelegate.fIndex)
                            Layout.preferredWidth: 24
                            Layout.preferredHeight: 24
                        }
                    }

                    // Show slider for Gamma, Log
                    ColumnLayout {
                        visible: filterDelegate.fName === "Gamma" || filterDelegate.fName === "Log"
                        Layout.fillWidth: true
                        spacing: 2
                        RowLayout {
                            Label { text: filterDelegate.fName === "Gamma" ? "Gamma:" : "C:"; color: "white" }
                            Label { text: filterDelegate.fValue !== undefined ? filterDelegate.fValue.toFixed(2) : "1.00"; color: "#aaa" }
                        }
                        Slider {
                            from: 0.01
                            to: 5.0
                            value: filterDelegate.fValue !== undefined ? filterDelegate.fValue : 1.0
                            Layout.fillWidth: true
                            onMoved: imageController.updateFilterValue(filterDelegate.fIndex, value)
                        }
                    }

                    // Contrast points
                    ColumnLayout {
                        visible: filterDelegate.fName === "Contrast"
                        Layout.fillWidth: true
                        spacing: 4

                        Repeater {
                            model: filterDelegate.fName === "Contrast" ? filterDelegate.fPoints : []
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 1
                                RowLayout {
                                    spacing: 5
                                    Label { text: "Point " + index + ": (" + modelData.r + ", " + modelData.s + ")"; color: "white"; font.pixelSize: 10 }
                                    Item { Layout.fillWidth: true }
                                    Button {
                                        text: "✕"
                                        flat: true
                                        onClicked: imageController.removeContrastPoint(filterDelegate.fIndex, modelData.index)
                                        visible: filterDelegate.fPoints && filterDelegate.fPoints.length > 2 && index > 0 && index < filterDelegate.fPoints.length - 1
                                        Layout.preferredWidth: 16
                                        Layout.preferredHeight: 16
                                        font.pixelSize: 10
                                    }
                                }
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 5
                                    Label { text: "R"; color: "#888"; font.pixelSize: 10 }
                                    Slider {
                                        from: 0; to: 255; value: modelData.r
                                        enabled: index > 0 && index < (filterDelegate.fPoints ? filterDelegate.fPoints.length - 1 : 0)
                                        Layout.fillWidth: true
                                        onMoved: imageController.updateContrastPoint(filterDelegate.fIndex, modelData.index, value, modelData.s)
                                    }
                                    Label { text: "S"; color: "#888"; font.pixelSize: 10 }
                                    Slider {
                                        from: 0; to: 255; value: modelData.s
                                        Layout.fillWidth: true
                                        onMoved: imageController.updateContrastPoint(filterDelegate.fIndex, modelData.index, modelData.r, value)
                                    }
                                }
                            }
                        }
                        Button {
                            text: "+ Add Point"
                            onClicked: imageController.addContrastPoint(filterDelegate.fIndex, 128, 128)
                            Layout.fillWidth: true
                            visible: filterDelegate.fName === "Contrast"
                        }
                    }
                }
            }
        }

        Button {
            text: "Clear All"
            Layout.fillWidth: true
            onClicked: imageController.clearFilters()
            visible: filterListView.count > 0
        }
    }
}
