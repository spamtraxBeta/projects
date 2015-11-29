/******************************************************************************
* This file is part of ImageTransformer.                                      *
*                                                                             *
* RelaisCard is free software : you can redistribute it and / or modify       *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation, either version 3 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* RelaisCard is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                 *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with RelaisCard.If not, see <http://www.gnu.org/licenses/>.           *
*******************************************************************************/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0
import QtQuick.Layouts 1.0
import MaEr 1.0


ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    statusBar: StatusBar{
        RowLayout {
            anchors.fill: parent
            Label {id: statusLabel; text: mainForm.image1.source }
        }
    }

    MainForm {
        id: mainForm
        anchors.fill: parent
        buttonOpen.onClicked: fileDialog.open();
        buttonSave.onClicked: {
            console.log("SAVE")
        }

    }

    Settings{
        id: settings
        category: "choose"
        property alias folder: fileDialog.folder
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a directory")
        folder: shortcuts.home
        selectFolder: true
        onAccepted: {
            dirIterator.dir = fileDialog.folder
        }
        onRejected: {
        }

    }

    MDirIterator{
        id: dirIterator

        fileTypes: ["*.jpg", "*.bmp"]

        onCurrentFileChanged: {
            action.save = true
            action.source = currentFile
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    Item {
        id: keyReceiver
        anchors.fill: parent
        focus: true
        Keys.onPressed: {

            switch(event.key){

            case Qt.Key_Right:
                dirIterator.next()
                break;

            case Qt.Key_H:
                action.flipH = !action.flipH
                break;

            case Qt.Key_V:
                action.flipV = !action.flipV
                break;

            case Qt.Key_R:
                action.rot90cw = !action.rot90cw
                break;

            case Qt.Key_T:
                action.rot180cw = !action.rot180cw
                break;

            case Qt.Key_Z:
                action.rot270cw = !action.rot270cw
                break;

            case Qt.Key_Q:
                Qt.quit()
                break;
            }

        }
    }

    Item{
        id: action
        property bool flipH
        property bool flipV
        property bool rot90cw
        property bool rot180cw
        property bool rot270cw
        property bool save;

        function reset()
        {
            action.flipH = false;
            action.flipV = false;
            action.rot90cw = false;
            action.rot180cw = false;
            action.rot270cw = false;
            action.save = false
        }

        property string source: ""

        onSourceChanged: {
            action.reset()
            actionChanged()
        }


        onFlipHChanged: actionChanged()
        onFlipVChanged: actionChanged()
        onRot90cwChanged: actionChanged()
        onRot180cwChanged: actionChanged()
        onRot270cwChanged: actionChanged()
        onSaveChanged: actionChanged()
        function actionChanged()
        {
            mainForm.image1.source = action.genSource();
        }




        function genSource()
        {
            var actions = [];
            var counter = 0;

            if (action.flipH)
            {
                actions[counter++] = "flipH"
            }

            if (action.flipV)
            {
                actions[counter++] = "flipV"
            }

            if (action.rot90cw)
            {
                actions[counter++] = "rot90cw"
            }

            if (action.rot180cw)
            {
                actions[counter++] = "rot180cw"
            }

            if (action.rot270cw)
            {
                actions[counter++] = "rot270cw"
            }

            if (action.save)
            {
                actions[counter++] = "save"
            }
            var actionString = actions.join(",");
            var result = "image://transform/%1::%2".arg(action.source).arg(actionString)
            return result;
        }
    }

    Image{
        id: img


    }
}

