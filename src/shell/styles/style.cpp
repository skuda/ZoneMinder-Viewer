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
#include "style.h"

#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QDir>


class Style::Private
{
    public:
        Private( ):setButton( 0 ),isValid(true){}
        bool canManageStyleVersion ( const QString & version )
        {
            if ( version.isNull() )
            {
                qDebug ( "no version for style: %s", qPrintable ( xmlFile ) );
                return true;
            }
            if ( version == "0.1" ) return true;
            return false;
        }
        QString name;
        QString author;
        QString authorEmail;
        QString styleSheetFile;
        QString style;
        QString version;
        QString xmlFile;
        bool isValid;
        QPushButton * setButton;
};

Style::Style ( const QString & xmlFile, QObject * parent )
        :QObject(parent) , d ( new Private )
{
    qDebug ( "Style::Style: xml file: %s", qPrintable ( xmlFile ) );
    d->xmlFile = xmlFile;
    init();
}

void Style::init()
{
    QFile file ( d->xmlFile );
    if ( !file.open ( QIODevice::ReadOnly ) )
    {
        qDebug ( "Style::init: could not open xml file: %s", qPrintable ( d->xmlFile ) );
        d->isValid = false;
        return;
    }
    QXmlSimpleReader reader;
    QXmlInputSource source;
    source.setData ( file.readAll() );
    QString errorMsg;
    int line,column = 0;
    QDomDocument document;
    if ( !document.setContent ( &source, &reader, &errorMsg, &line , &column ) )
    {
        qWarning ( "Invalid Style Format: %s at line %d , column %d", qPrintable ( errorMsg ), line, column );
        d->isValid = false;
        return;
    }
    QDomElement docElem = document.documentElement();

    QDomNode n = docElem.firstChild();
    if ( docElem.tagName() =="zmviewerstyle" && !d->canManageStyleVersion ( docElem.attribute ( "version" ) ) )
    {
        qWarning ( "Style::init():Style version not supported" );
        d->isValid = false;
        return;
    }
    while ( !n.isNull() )
    {
        parseStyle ( n.toElement() );
        n = n.nextSibling();
    }


}

void Style::parseStyle ( const QDomElement & e )
{
    if ( e.tagName() == "name" )
    {
        d->name = e.text();
    }
    else if ( e.tagName() == "version" )
    {
        d->version = e.text();
    }
    else if ( e.tagName() == "author" )
    {
        d->author = e.text();
    }
    else if ( e.tagName() == "authoremail" )
    {
        d->authorEmail = e.text();
    }
    else if ( e.tagName() == "style" )
    {
        d->style = e.text();
    }
    else if ( e.tagName() == "stylesheet" )
    {
        d->styleSheetFile = e.text();
    }
}

QString Style::name() const
{ 
    return d->name;
}
QString Style::version() const
{
    return d->version;
}
QString Style::author() const{
    return d->author;
}
QString Style::authorEmail() const{
    return d->authorEmail;
}
QString Style::style() const{
    return d->style;
}
QString Style::styleSheetName() const{
    return d->styleSheetFile;
}
QString Style::styleSheet() const{
    QFileInfo info ( d->xmlFile );
    QString fileAbs = info.path() + QDir::separator() + d->styleSheetFile ;
    QFile file ( fileAbs );
    if ( !file.open( QFile::ReadOnly ) ){
            qWarning ("Style::styleSheet():qss file %s not found", qPrintable (fileAbs ) );
            return QString();
    }
    return file.readAll();
}

QString Style::fileName()const{
    return d->xmlFile;
}

bool Style::isValid() const{
    return d->isValid;
}

Style::Style( const Style & other)
:QObject( other.parent() ),d( new Private( *other.d) )
{
}

Style Style::operator=( const Style & other){
  this->d = new Private(*other.d);
  return *this;
};


Style::~Style()
{
  delete d;
}


#include "style.moc"
