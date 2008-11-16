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
#ifndef REQUESTINTERFACE_H
#define REQUESTINTERFACE_H

#include <QObject>
#include <QList>

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/

class Stream;
class RequestInterface : public QObject{
    Q_OBJECT
public:
    RequestInterface( QObject * parent = 0 );

    virtual QList <Stream *> streams() = 0;
    virtual bool deleteEvent( int eventId ) = 0;

    ~RequestInterface();

};

#endif
