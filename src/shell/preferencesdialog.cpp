/***************************************************************************
 *   Copyright (C) 2007 by Leandro Emanuel López   *
 *   lopezlean@gmail.com   *
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

#include "preferencesdialog.h"

#include "config/cameraconfigwidget.h"
#include "config/languageconfigwidget.h"

#include <QSettings>

PreferencesDialog::PreferencesDialog( QWidget * parent )
:ConfigDialog( parent )
{ 
    init();
}

void PreferencesDialog::init(){
    CameraConfigWidget * cameraConfig = new CameraConfigWidget ( this );
    LanguageConfigWidget * languageConfig = new LanguageConfigWidget( this );

    addWidget ( cameraConfig );
    addWidget ( languageConfig );

    readSettings();
}

void PreferencesDialog::dialogAccepted(){
    ConfigDialog::dialogAccepted();
    saveSettings();
}

void PreferencesDialog::readSettings(){
    QSettings settings; 
    settings.beginGroup( "Preferences" );
    QSize size = settings.value( "preferencesDialogSize", QSize( 640, 480 ) ).toSize();
    settings.endGroup();
    resize( size );

}
void PreferencesDialog::saveSettings(){
    QSettings settings; 
    settings.beginGroup( "Preferences" );
    settings.setValue( "preferencesDialogSize", size() );
    settings.endGroup();
}

PreferencesDialog::~PreferencesDialog()
{
}

#include "preferencesdialog.moc"
