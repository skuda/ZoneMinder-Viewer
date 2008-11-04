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

#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include "basemodel.h"
/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
class EventModel: public BaseModel{
    Q_OBJECT
public:
        enum Fields{
        Id,
        MonitorId,
        Name,
        Cause,
        StartTime,
        EndTime,
        Width,
        Height,
        Length,
        Frames,
        AlarmFrames,
        TotScore,
        AvgScore,
        MaxScore,
        Archivied,
        Videoed,
        Uploaded,   
        Emailed,
        Messaged,
        Executed,
        LearnState,
        Notes
    };
    EventModel( const QString &cn, QObject * parent = 0 );
    void setFilter ( const QString & filter );
    void startEventTracker();
    void stopEventTracker();
    ~EventModel();
protected Q_SLOTS:
    void searchForNewEvents();
Q_SIGNALS:
    void eventsDetected( int newsEventCount );
private:
    class Private;
    Private *d;

};

#endif
