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
#ifndef AUTH_H
#define AUTH_H

/**
	@author Leandro Emanuel Lopez <lopezlean@gmail.com>
*/
#include <QObject>
#include <QString>
#include <QByteArray>

class Auth : public QObject{
Q_OBJECT

public:
    Auth( const QString & db , QObject * parent = 0 );
    enum AuthType{PLAIN,HASHED,NONE};
    int authType() const;
    bool isAuthNeeded() const;
    bool isAuth() const;
    ~Auth();
    bool userLogin( const QString &username , const QString &password );
    QString zmsString();
    QByteArray authKey();

private:
    void init();
    void saveSettings();
    void loadSettings();
    const QByteArray calculateAuthKey();

    QString m_db;
    bool m_isAuth;
    AuthType m_AuthType;
    bool m_needAuth;
    QString m_userName;
    QString m_password;
    QString m_hashPassword;
    QByteArray m_authKey;
};

#endif
