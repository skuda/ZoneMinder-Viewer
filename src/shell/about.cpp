/***************************************************************************
 *   Copyright (C) 2007 by Leandro Emanuel López                           *
 *   lopezlean@gmail.com                         *
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
#include "about.h"
#include <config-zmviewer.h>

#include <QDialog>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QApplication>
#include <QTextBrowser>
#include <QObject>

About::About ( )
{
}

void About::showAboutDialog()
{
    m_dialog = new QDialog ( QApplication::focusWidget() );
    m_dialog->setWindowTitle ( QObject::tr( "About..." ) );
    QVBoxLayout * layout = new QVBoxLayout ( m_dialog );

    QHBoxLayout * titleLayout = new QHBoxLayout ( );
    QLabel * programName = new QLabel ( QString ( "<b>" ) + QObject::tr(ApplicationName) + QString ( " " ) +QString( ZMVIEWER_VERSION_STRING ) + QString ( "</b>" ), m_dialog );
    titleLayout->addWidget ( programName );
    QLabel * programIcon = new QLabel ( m_dialog );
    programIcon->setPixmap ( qApp->windowIcon().pixmap ( 32,32 ) );
    titleLayout->addWidget ( programIcon );
    layout->addLayout ( titleLayout );

    QTabWidget * tab = new QTabWidget ( m_dialog );
    QLabel * label = new QLabel ( aboutText (), m_dialog );
    tab->addTab ( label , QObject::tr( "About" ) );

    QTextBrowser * authors = new QTextBrowser ( m_dialog );
    authors->setHtml ( authorsText () );
    authors->setReadOnly ( true );
    authors->setOpenExternalLinks ( m_dialog );

    tab->addTab ( authors , QObject::tr( "Authors and Thanks" ) );


    layout->addWidget ( tab );
    QPushButton * button = new QPushButton ( QObject::tr( "Ok" ) , m_dialog );
    QHBoxLayout * buttonLayout = new QHBoxLayout ( );
    buttonLayout->addItem ( new QSpacerItem ( 20,30, QSizePolicy::Expanding ) );
    buttonLayout->addWidget ( button );
    layout->addLayout ( buttonLayout );

    QObject::connect ( button, SIGNAL ( clicked() ),m_dialog,SLOT ( accept() ) );
    m_dialog->exec();
}


/*static char * About::applicationName(){
    return qPrintable(QObject::QObject::tr( "ZoneMinder Viewer" ));
}*/

QString About::applicationName()
{
    return QObject::tr( ApplicationName );
}

QString About::versionString()
{
    return ZMVIEWER_VERSION_STRING;
}

QString About::aboutText()
{
    return QObject::tr( "%1 is a zoneminder client written in Qt Framework.\nCopyright(c)2007 Leandro Emanuel López\n").arg(ApplicationShortName );
}

QString About::authorsText(){
    return QObject::tr("<b>Developers:</b><br/><ul>\
                       <li>Leandro Emanuel López ( Maintainer and main developer ) <br>\
                       <a href=mailto:lopezlean@gmail.com>lopezlean@gmail.com</a><br></li>\
                       <li>Dardo Sordi Bogado ( Original idea and ZoneMinder Stream connection ) <br>\
                       <a href=mailto:dardo.sordi@gmail.com>dardo.sordi@gmail.com</a> </li></ul><br>\
                       <b>Thanks to:</b><ul>\
                       <li>SILIX ( ZMViewer was developed here ) <br>\
                       <a href=http://www.silix.com.ar>http://www.silix.com.ar</a> </li>\
                       <li>Paweł Porwolik ( Language Menu and Polish translation ) <br>\
                       <a href=mailto:maxen1@wp.pl>maxen1@wp.pl</a> </li>\
                        <li>Michele Perrucci ( Italian translation ) <br>\
                       <a href=mailto:michele.perrucci@startechsnc.it>michele.perrucci@startechsnc.it</a> </li>\
                        </ul><br></ul>\
                       ");
}
About::~About()
{
}


