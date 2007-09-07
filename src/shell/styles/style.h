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
#ifndef STYLE_H
#define STYLE_H

/**
	@author Leandro Emanuel Lopez <lopezlean@gmail.com>
*/
#include <QString>
#include <QObject>
class QDomElement;
class QPushButton;
class Style: public QObject{
Q_OBJECT
public:
    Style( const QString & xmlFile , QObject * parent = 0);
    Style( const Style & other);
    /**
        @return style's name
    */
    QString name() const;
    /**
       @return style's version
    */
    QString version() const;
    /**
        @return style's author
    */
    QString author() const;
    /**
        @return style's author email
    */
    QString authorEmail() const;
    /**
        @return style( QStyle )
    */
    QString style() const;
    /**
        @return style sheet's name
    */
    
    QString styleSheetName() const;
    /**
        @return style sheet's contents
    */
    QString styleSheet() const;
    bool fullscreen() const;
    
    Style operator=( const Style & other);
    ~Style();

private:
    void init();
    void parseStyle( const QDomElement & e );
    class Private;
    Private *d;

};

#endif
