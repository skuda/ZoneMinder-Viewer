/***************************************************************************
 *   Copyright (C) 2007 by Leandro Emanuel López                           *
 *   lopezlean@gmail.com  				                   *
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
#include "adminpanel.h"
#include <QUrl>
#include <QInputDialog>
#include <QSettings>

AdminPanel::AdminPanel( const QString &host, const QString &settingGroup, QWidget * parent )
:QWebView( parent ),m_popupView(0)
{
    resize(QSize(750, 462));
    setHost( host, settingGroup );
}

void AdminPanel::setHost( const QString &host, const QString &settingGroup ){
    #if QT_VERSION >= 0x040400
     bool okPressed = false;
     QString url = QInputDialog::getText(0,tr("ZMViewer - Where is de ZoneMinder Web Root location?"), tr("ZoneMinder Default Administration Interface Root Location"), QLineEdit::Normal, host , &okPressed);
     load( QUrl( url + "/zm.php") );
     QSettings s;
     s.beginGroup( settingGroup );
     s.setValue("adminPanel", url );
     s.endGroup();

     setWindowTitle( tr( "ZMViewer - %1 Admin Panel" ).arg( QUrl(url).host() ) );

     #endif

}

QWebView * AdminPanel::createWindow ( QWebPage::WebWindowType type ){
    if ( !m_popupView ){
        m_popupView = new QWebView;
    }

    return m_popupView;
}

AdminPanel::~AdminPanel()
{
    delete m_popupView;
}

#include "adminpanel.moc"
