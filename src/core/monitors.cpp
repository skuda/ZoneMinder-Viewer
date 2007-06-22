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
#include "monitors.h"
#include "connectionmanager.h"
#include "camerawidget.h"
#include "stream.h"
#include "config.h"

#include<QSqlQuery>
#include<QStringList>
#include<QVariant>
#include<QSqlRecord>



Monitors::Monitors(QObject * parent )
:QObject( parent )
{
    init();
}


void Monitors::init(){
    foreach (QString  connection , ConnectionManager::connectionNames() ){
        QSqlDatabase db = QSqlDatabase::database( connection ) ;
        QSqlQuery query = db.exec("SELECT Value from Config where Name='ZM_PATH_ZMS'");
        query.next();
        QString zms = query.value(0).toString();
        query.clear();
        query = db.exec("SELECT * from Monitors where 1");
        while (query.next()) {
            CameraWidget * camera = new CameraWidget (  connection );
            camera->setWindowTitle( query.value(query.record().indexOf("Name")).toString() + _(" at ") + db.hostName() );
            camera->stream()->setHost( db.hostName() ,query.value(query.record().indexOf("Port")).toInt() );
            camera->stream()->setMonitor( query.value(query.record().indexOf("Id")).toUInt() );
            camera->stream()->setZMStreamServer( zms );
            m_cameras.append( camera );
            camera->setAutoAdjustImage ( true );
            camera->startCamera();
        }
        query.clear();
    }

}

int Monitors::count(){
    return m_cameras.count();
}

void Monitors::updateMonitors(){
    m_cameras.clear();
    init();
}

Monitors::~Monitors()
{
}

#include "monitors.moc"


