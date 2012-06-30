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
#include <QtCore/QTimer>
#include <QtNetwork/QtNetwork>
#include <QtCore/QtCore>

AdminPanel::AdminPanel(QWidget * parent ) :QWebView( parent )
{
}

bool AdminPanel::setHost( const QString &host, const QString &settingGroup ){
    bool okPressed = false;

#if QT_VERSION >= 0x040400
     QString urlstr = QInputDialog::getText(0, tr("ZMViewer - Where is the ZoneMinder Web Root location?"),
                                            tr("ZoneMinder Default Administration Interface Root Location"),
                                            QLineEdit::Normal, host , &okPressed);

     QUrl url( urlstr + "/index.php?skin=classic");
     url.addQueryItem("action", "login");
     url.addQueryItem("view", "options");
     //TODO: change this to get username and pass from auth class.
     url.addQueryItem("username", "admin");
     url.addQueryItem("password", "secret");

     QNetworkRequest request(url);
     request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html");
     load(request, QNetworkAccessManager::PostOperation, url.encodedQuery());

     QSettings s;
     s.beginGroup( settingGroup );
     s.setValue("adminPanel", urlstr );
     s.endGroup();

     setWindowTitle( tr( "ZMViewer - %1 Admin Panel" ).arg( url.host() ) );
#endif

     return okPressed;
}

QWebView * AdminPanel::createWindow ( QWebPage::WebWindowType /*type*/ ){
    return this; //this is how we handle pop-up windows
}

void AdminPanel::show()
{
    QWebView::show();
    QTimer::singleShot(0, this, SLOT(resizeToConfigSize()));
}

void AdminPanel::resizeToConfigSize()
{
    resize(950, 700);
}

AdminPanel::~AdminPanel()
{
}
