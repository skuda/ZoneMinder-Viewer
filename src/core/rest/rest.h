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

#ifndef REST_H
#define REST_H

#include <QObject>
class QUrl;
class RequestInterface;
/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
        @brief Rest Object to access Zoneminder
*/
//http://localhost/zm/index.php?view=none&action=delete&mark_eid=22


class Rest: public QObject{
    Q_OBJECT
public:
    Rest( const QUrl &host, QObject * parent = 0 );
    void setRequestObject ( RequestInterface * iface );
    RequestInterface * request ( ) const;
    ~Rest();
private:
    class Private;
    Private *d;
    void init();

};

#endif
