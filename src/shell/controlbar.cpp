/***************************************************************************
 *   Copyright (C) 2008 by Leandro Emanuel López                           *
 *   lopezlean@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "controlbar.h"
#include "cameraviewer.h"

#include <QActionGroup>

class ControlBar::Private{
public:
    CameraViewer * cameraViewer;
    QActionGroup * camerasFocusActions;
};

ControlBar::ControlBar( QWidget * parent )
:QToolBar( parent ), d( new Private )
{
    init();
}

void ControlBar::init(){
    setWindowTitle ( tr("Control ToolBar") );
    d->cameraViewer = CameraViewer::instance();
    d->camerasFocusActions = 0;

    d->camerasFocusActions = d->cameraViewer->cameraFocusActions();
    updateCameraFocusActions();

}

void ControlBar::updateCameraFocusActions(){
    if ( !d->camerasFocusActions ) return;
    foreach( QAction * action, d->camerasFocusActions->actions() ){
        //action->setShortcut( QKeySequence( action->text() ) );
        action->shortcut().toString();
        addAction( action );
    }
}
void ControlBar::removeCameraFocusActions(){
    if ( !d->camerasFocusActions ) return;

    foreach( QAction * action, d->camerasFocusActions->actions() ){
        removeAction( action );
    }
}

void ControlBar::updateControls(){
    removeCameraFocusActions();
    d->camerasFocusActions = d->cameraViewer->cameraFocusActions();
    updateCameraFocusActions();
}

ControlBar::~ControlBar()
{
    delete d;
}

