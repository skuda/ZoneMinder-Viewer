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

#include "rest.h"
#include "requestinterface.h"
#include "zoneminderhtmlrequest.h"

#include <QUrl>
class Rest::Private{
public:
    QUrl host;
    RequestInterface * requestObject;
};
Rest::Rest( const QUrl &host, QObject * parent )
:QObject ( parent ), d( new Private ){
    Q_ASSERT( host.isValid() );
    d->host = host;
}

void Rest::init(){
        d->requestObject = new ZoneMinderHtmlRequest ( this );
}

void Rest::setRequestObject ( RequestInterface * iface ){
    d->requestObject = iface;
}

RequestInterface * Rest::request ( ) const{
    return d->requestObject;
}

Rest::~Rest()
{
}

#include "rest.moc"
