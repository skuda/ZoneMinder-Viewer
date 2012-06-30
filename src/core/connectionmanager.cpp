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
#include <QString>
#include <QObject>
#include <QSettings>


static QSqlError lastError;
static QMap<QString, int > cnPorts;

ConnectionManager::ConnectionManager()
{}


bool ConnectionManager::addConnection ( const QString &d , const QString & h, const QString & dn , const QString & u,const QString & p , int port, int wwwPort ,bool removeiferror )
{
    //driver,host,database,user,port
    QString cn = d+h+dn+u+QString::number ( port );
    if ( QSqlDatabase::contains ( cn ) )
    {
        lastError.setDatabaseText ( QObject::tr ( "Connection to <b>%1</b> already exist at host %2" ).arg ( dn ).arg ( h ) );
        return false;
    }
    bool dbOpen = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase ( d , cn );
        db.setHostName ( h );
        db.setDatabaseName ( dn );
        db.setUserName ( u );
        db.setPassword ( p );

        if ( port != 0 )
            db.setPort ( port );
        dbOpen = db.open();
        if ( ! dbOpen ) 
            lastError = db.lastError();
        else 
            lastError = QSqlError(); //no error
    }
    if ( ! dbOpen )
    {
        if( removeiferror )
            QSqlDatabase::removeDatabase ( cn );
    }
    else
    {
        saveConnection ( d , h , dn , u , p , port, wwwPort );
    }
    return dbOpen;
}

QStringList ConnectionManager::connectionNames()
{
    return QSqlDatabase::connectionNames();
}

void ConnectionManager::closeAll(){
    QStringList cnList = ConnectionManager::connectionNames();
    foreach ( QString cn, cnList ){
        QSqlDatabase::removeDatabase(cn);
    }
}

bool ConnectionManager::constains ( const QString & name )
{
    return QSqlDatabase::contains ( name );
}

QString ConnectionManager::lastErrorString()
{
    return lastError.databaseText();
}

void ConnectionManager::saveConnection ( const QString &d , const QString & h, const QString & dn , const QString & u,const QString & p , int port, int wwwPort )
{
    QString cn = d+h+dn+u+QString::number ( port );
    QSettings s;
    s.beginGroup ( "Databases" );
    QStringList v = s.value ( "names" ).toStringList();
    if ( v.indexOf( cn ) == -1 )
        v.append ( cn );
    s.setValue ( "names", v );
    s.endGroup();
    s.beginGroup ( cn );
    s.setValue( "driver", d );
    s.setValue( "host", h );
    s.setValue( "database", dn );
    s.setValue( "user", u );
    s.setValue( "password", p );
    s.setValue( "port", port);
    s.setValue( "wwwPort", wwwPort );
    s.endGroup();

    cnPorts.insert( cn , wwwPort );

}

void ConnectionManager::removeConnectionWebPort( const QString & cn ){
    cnPorts.remove( cn );
}

int ConnectionManager::connectionWebPort( const QString & cn ){
    return cnPorts.value( cn );
}

ConnectionManager::~ConnectionManager()
{}


