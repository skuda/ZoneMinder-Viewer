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

#include "auth.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

#include <QSettings>

Auth::Auth ( const QString & db , QObject * parent )
        :QObject ( parent ),m_isAuth(false)
{
    m_db = db;
    init();
}

void Auth::init()
{
    QSqlDatabase db = QSqlDatabase::database ( m_db );
    QSqlQuery query = db.exec ( "SELECT Value from Config where Name='ZM_OPT_USE_AUTH'" );
    query.next();
    m_needAuth = query.value ( 0 ).toBool();
    query.clear();
    query = db.exec ( "SELECT Value from Config where Name='ZM_AUTH_RELAY'" );
    query.next();
    QString StringType = query.value ( 0 ).toString();
    query.clear();
    if ( StringType.toLower() == "hashed" )
        m_AuthType = HASHED;
    else if ( StringType.toLower() == "plain" )
        m_AuthType = PLAIN;
    else m_AuthType = NONE;

    loadSettings();
    userLogin( m_userName , m_password );
        
    



}

bool Auth::userLogin( const QString &username , const QString &password ){
    if ( m_AuthType == NONE ){
            qDebug("userLogin::Try to login with NONE ? Please Fix this.");
            return m_isAuth = true;
    }
    if ( m_AuthType == HASHED){
            qDebug("userLogin::HASHED Auth not implemented yet!");
            return m_isAuth = true;
    }
    QSqlDatabase db = QSqlDatabase::database ( m_db );
    QSqlQuery query = db.exec ( QString("SELECT * from Users where Username = '%1' and Password = password('%2') and Enabled = 1" ).arg(username).arg(password));
    query.next();
    if( query.size() != 1 )
        return m_isAuth = false;
    
    /* OK */
    m_userName = username;
    m_password = password;
    saveSettings();
    return m_isAuth = true;
}

QString Auth::zmsString( ) const{
    QString r("user="+m_userName+"&pass="+m_password);
    return r;
}

int Auth::authType() const
{
    return m_AuthType;
}
bool Auth::isAuthNeeded() const
{
    return m_needAuth;
}

bool Auth::isAuth() const{
    return m_isAuth;
}

void Auth::loadSettings(){
    QSettings s;
    s.beginGroup( m_db );
    m_userName = s.value( "AuthUser" ).toString();
    m_password = s.value( "AuthPassword").toString();
    s.endGroup();

}

void Auth::saveSettings(){
    QSettings s;
    s.beginGroup ( m_db );
    s.setValue( "AuthUser", m_userName );
    s.setValue( "AuthPassword", m_password );
    s.endGroup();
}

Auth::~Auth()
{}


#include "auth.moc"
