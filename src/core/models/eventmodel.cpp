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
#include "eventmodel.h"

#include <QTimer>

class EventModel::Private{
    public:
        int lastEventCount;
        bool eventTracker;
        QTimer timer;
        int cameraId;
};

EventModel::EventModel( const QString &cn, QObject * parent )
:BaseModel( cn, parent ),d( new Private ){
    d->lastEventCount = 0;
    d->eventTracker = false;
    connect( &d->timer, SIGNAL(timeout()), this, SLOT(searchForNewEvents()));
}

void EventModel::setCamera( int id ){
    d->cameraId = id;
    setFilter( "MonitorId = " + QString::number ( d->cameraId ) );
    setTable( "Events" );
}

void EventModel::startEventTracker( ){
    setFilter( "MonitorId = " + QString::number ( d->cameraId ) );
    select();
    d->lastEventCount = rowCount();
    d->eventTracker = true;
    d->timer.start( 5000 );
}

void EventModel::stopEventTracker(){
    d->lastEventCount = 0;
    d->eventTracker = false;
    d->timer.stop();
}

void EventModel::searchForNewEvents(){
    select();
    int lastRowCount = d->lastEventCount;
    int newRowCount = d->lastEventCount = rowCount();
    if ( newRowCount > lastRowCount ){
        emit ( eventsDetected( newRowCount - lastRowCount ) );
    }
}

EventModel::~EventModel()
{
    delete d;
}



