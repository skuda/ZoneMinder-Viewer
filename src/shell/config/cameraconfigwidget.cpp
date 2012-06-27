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
#include "cameraconfigwidget.h"

#include "cameraviewer.h"

#include <QSettings>
CameraConfigWidget::CameraConfigWidget( QWidget * parent )
:ConfigDialogPage( parent )
{
    setupUi( this );
    setIcon( QIcon(":/config/CameraConfig") );
    setTitle( tr("Cameras") );
    setSubTitle( tr("Configure your cameras") );

    m_aspectRatioComboBox->addItem( tr("Ignore"), Qt::IgnoreAspectRatio );
    m_aspectRatioComboBox->addItem( tr("Keep"), Qt::KeepAspectRatio );
    m_aspectRatioComboBox->addItem( tr("Keep by Expanding"), Qt::KeepAspectRatioByExpanding );

    m_cameraNumbersPositionComboBox->addItem( tr("Top Right"), (int)(Qt::AlignRight | Qt::AlignTop) );
    m_cameraNumbersPositionComboBox->addItem( tr("Top Left") , (int)(Qt::AlignLeft | Qt::AlignTop) );
    m_cameraNumbersPositionComboBox->addItem( tr("Center"), (int)(Qt::AlignVCenter | Qt::AlignHCenter) );
    m_cameraNumbersPositionComboBox->addItem( tr("Bottom Right"), (int)(Qt::AlignRight | Qt::AlignBottom) );
    m_cameraNumbersPositionComboBox->addItem( tr("Bottom Left"), (int)(Qt::AlignLeft | Qt::AlignBottom) );

    readSettings();
}

bool CameraConfigWidget::dialogAccepted(){
    writeSettings();
    return true;
}

void CameraConfigWidget::readSettings(){
    QSettings settings;
    settings.beginGroup( "Preferences" );
    Qt::AspectRatioMode aspectRatio =  (Qt::AspectRatioMode)settings.value( "aspectRatio",  Qt::KeepAspectRatio ).toInt();
    bool showCameraNumbers = settings.value( "showCameraNumber", true ).toBool();
    Qt::Alignment alignCameras =  ( Qt::Alignment ) settings.value( "cameraNumberPosition", (int)(Qt::AlignRight | Qt::AlignTop)  ).toInt();

    settings.endGroup();
    int aspectDataIndex = m_aspectRatioComboBox->findData ( aspectRatio );
    if ( aspectDataIndex < 0 ) aspectDataIndex = 0;

    m_aspectRatioComboBox->setCurrentIndex( aspectDataIndex );
    m_showCameraNumbersCheckBox->setChecked( showCameraNumbers );

    int cameraNumberPositionIndex = m_cameraNumbersPositionComboBox->findData ( (int)alignCameras );
    if (cameraNumberPositionIndex < 0 ) cameraNumberPositionIndex = 0;
    m_cameraNumbersPositionComboBox->setCurrentIndex( cameraNumberPositionIndex );

}
void CameraConfigWidget::writeSettings(){
    QSettings settings;
    settings.beginGroup("Preferences");
    settings.setValue( "aspectRatio", m_aspectRatioComboBox->itemData ( m_aspectRatioComboBox->currentIndex() ) );
    settings.setValue( "showCameraNumber", m_showCameraNumbersCheckBox->isChecked() );
    settings.setValue( "cameraNumberPosition", m_cameraNumbersPositionComboBox->itemData( m_cameraNumbersPositionComboBox->currentIndex()  ) );
    settings.endGroup();
    Qt::AspectRatioMode aspectRatio = (Qt::AspectRatioMode)m_aspectRatioComboBox->itemData ( m_aspectRatioComboBox->currentIndex() ).toInt();
    bool showCameraNumbers = m_showCameraNumbersCheckBox->isChecked();
    Qt::Alignment alignCameras =  ( Qt::Alignment ) m_cameraNumbersPositionComboBox->itemData( m_cameraNumbersPositionComboBox->currentIndex() ).toInt();
    CameraViewer::instance()->setCameraOptions(  aspectRatio, showCameraNumbers, alignCameras );

}

CameraConfigWidget::~CameraConfigWidget()
{
}
