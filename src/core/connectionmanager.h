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
#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H


class QString;
class QStringList;

/**
 @author Leandro Emanuel Lopez <lopezlean@gmail.com>
*/

class ConnectionManager
{
    public:
        ConnectionManager();
        ~ConnectionManager();
        /**
            driver, hostname , database name , user, password, port
        */
        static bool addConnection ( const QString &d , const QString & h, const QString & dn , const QString & u,const QString & p , int port = 0 );
        bool constains ( const QString & name );
        static QStringList connectionNames();

        static QString lastErrorString();
    private:
        static void saveConnection(const QString &d , const QString & h, const QString & dn , const QString & u,const QString & p , int port );
};

#endif
