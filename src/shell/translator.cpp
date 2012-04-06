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
#include "translator.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QSettings>

static Translator * m_instance = 0;

class Translator::Private{
public:
    QTranslator qtTranslator;
    QTranslator appTranslator;
    QString systemLocale;
    QString userLocale;
};

Translator::Translator( QObject * parent )
 : QObject( parent ),d( new Private )
{
    init();
}

void Translator::init(){
    d->systemLocale =  QLocale::system().name();
    loadSettings( );
    loadLanguage( d->userLocale );
}
Translator * Translator::instance(){
    if ( !m_instance )
        m_instance = new Translator;
    return m_instance;
}

QString Translator::currentLocale( ) const{
    return d->userLocale;
}

bool Translator::loadLanguage( const QString & localeString ){
    bool isDefault = localeString.left(2) == "en";

    if ( !isDefault ){
        if (!d->qtTranslator.load("qt_" + localeString , QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ) )
            qDebug ( qPrintable(QString("Translator::Can not load Qt Translations for locale %s" ).arg( localeString ) ) );
        else qApp->installTranslator( &d->qtTranslator );
    } else qApp->removeTranslator( &d->qtTranslator );
    bool loaded = false;
    if ( !isDefault )
        loaded = d->appTranslator.load(QString::fromUtf8(":/translations/zmviewer_") +  localeString );
    else loaded = true;
    if ( loaded ){
        if ( !isDefault )
            qApp->installTranslator(&d->appTranslator);
        else qApp->removeTranslator(&d->appTranslator);
        d->userLocale = localeString;
        emit( languageChanged( localeString ) );
    }
    return loaded;

}
void Translator::loadSettings(){
    QSettings settings;
    settings.beginGroup( "Translator" );
    d->userLocale = settings.value( "locale", d->systemLocale ).toString();
    settings.endGroup();
}

void Translator::writeSettings(){
    QSettings settings;
    settings.beginGroup( "Translator" );
    settings.setValue( "locale", d->userLocale );
    settings.endGroup();
}
Translator::~Translator()
{
}

#include "translator.moc"
