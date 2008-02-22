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
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
      QCoreApplication::setOrganizationName("SiliX");
      QCoreApplication::setOrganizationDomain("silix.com.ar");
      QCoreApplication::setApplicationName("zmviewer");
      QApplication app(argc, argv);
      #ifdef Q_WS_WIN
      QCoreApplication::addLibraryPath ( QCoreApplication::applicationDirPath () );
      #endif
      /* QT translations */
      QTranslator qtTranslator;
      if (!qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath))) qDebug ("Can not load Qt Translations");
      app.installTranslator(&qtTranslator);

      QTranslator translator;
      QString locale = QLocale::system().name();
      translator.load(QString::fromUtf8(":/translations/zmviewer_") + locale);
      app.installTranslator(&translator);
      MainWindow * m = new MainWindow;
      m->show();
      int r = app.exec();
      return r;
}

