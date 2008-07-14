/**************************************************************************
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

#include "cameraviewerpage.h"
#include "cameralayout.h"

CameraViewerPage::CameraViewerPage( int rows, int columns, QWidget * parent )
:QWidget( parent )
{
    m_cameraLayout = new CameraLayout( rows, columns );
    setSizePolicy( QSizePolicy::Expanding,QSizePolicy::Expanding );
    setLayout( m_cameraLayout );
}

void CameraViewerPage::appendCamera( QWidget * camera ){
        m_cameraLayout->addWidget( camera );
        camera->setFixedSize( m_cameraLayout->cellsSizeHint() );
}
void CameraViewerPage::updateLayout(){
    m_cameraLayout->updateLayout();
}

void CameraViewerPage::resizeEvent ( QResizeEvent * event ){
    setFixedSize( size() );
    updateLayout();
    QWidget::resizeEvent ( event );
}

bool CameraViewerPage::hasSpace() const{
    bool b = m_cameraLayout->hasSpace();

    return b;
}


CameraViewerPage::~CameraViewerPage()
{
}

#include "cameraviewerpage.moc"
