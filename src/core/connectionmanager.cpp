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

#include "connectionmanager.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>


static QSqlError lastError;

ConnectionManager::ConnectionManager()
{}


bool ConnectionManager::addConnection ( const QString &d , const QString & h, const QString & dn , const QString & u,const QString & p )
{
     QSqlDatabase db = QSqlDatabase::addDatabase( d );
     db.setHostName( h );
     db.setDatabaseName( dn );
     db.setUserName( u );
     db.setPassword( p );
     bool b = db.open();
     if ( ! b ) lastError = db.lastError();
     else lastError = QSqlError();
     return b;
}

QStringList ConnectionManager::connectionNames()
{
    return QSqlDatabase::connectionNames();
}

bool ConnectionManager::constains ( const QString & name ){
    return QSqlDatabase::contains( name );
}

QString ConnectionManager::lastErrorString(){
        return lastError.databaseText();
}


ConnectionManager::~ConnectionManager()
{}


