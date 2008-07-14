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

#ifndef ADMINPANEL_H
#define ADMINPANEL_H
#include <Qt>
#if QT_VERSION >= 0x040400
    #include <QtWebKit/QWebView>
#else
    #include <QMessageBox>
    #include <QWidget>
    class QUrl;
    class QWebView : public QWidget{
        public:
        QWebView( QWidget * parent = 0 ):QWidget( parent){};
        void load( const QUrl & url ){ return; }
        void show(){      QMessageBox::information ( 0, tr("Not available"), tr("You must compile with Qt 4.4 or later to use this feature"));}
    };
    class QWebPage{
        public:
        enum WebWindowType{};
     };
#endif

class AdminPanel: public QWebView {
    Q_OBJECT
public:
    AdminPanel( const QString &host, const QString &settingGroup ,QWidget * parent = 0 );
     void setHost( const QString &host, const QString &settingGroup );
    ~AdminPanel();
protected:
    QWebView * createWindow ( QWebPage::WebWindowType type );
private:
    QWebView * m_popupView;
};

#endif
