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
            driver, hostname , database name , user, password, port, wwwPort
        */
        static bool addConnection ( const QString &d , const QString & h, const QString & dn , const QString & u,const QString & p , int port = 0, int wwwPort = 80 , bool removeiferror = true );
        bool constains ( const QString & name );
        static QStringList connectionNames();
        static void closeAll();
        static QString lastErrorString();
        static void removeConnectionWebPort( const QString & cn );
        static int connectionWebPort( const QString & cn );
    private:
        static void saveConnection(const QString &d , const QString & h, const QString & dn , const QString & u,const QString & p , int port = 0, int wwwPort = 80  );
};

#endif
