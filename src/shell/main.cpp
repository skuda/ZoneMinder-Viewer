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


#include <QApplication>
#include <QSplashScreen>
#include <QLocale>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
      QCoreApplication::setOrganizationDomain("zmviewer.sf.net");
      QCoreApplication::setApplicationName("zmviewer");
      QApplication app(argc, argv);
      QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
      QSplashScreen splash( QPixmap(":/img/Splash"));
      splash.show();
      splash.showMessage( QObject::tr("Loading...") );
      app.processEvents();
      #ifdef Q_WS_WIN
      QCoreApplication::addLibraryPath ( QCoreApplication::applicationDirPath () );
      #endif

      MainWindow * m = new MainWindow;
      m->show();
      splash.finish( m );
      int r = app.exec();
      delete m;
      return r;
}

