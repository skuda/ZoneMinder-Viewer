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
#include "monitors.h"
#include "connectionmanager.h"
#include "camerawidget.h"
#include "stream.h"
#include "auth.h"
#include "authdialog.h"

#include<QSqlQuery>
#include<QStringList>
#include<QPair>
#include<QVariant>
#include<QSqlRecord>


static QList<QPair < QString,QString> > hostList;
Monitors::Monitors ( QObject * parent )
        :QObject ( parent )
{
    init();
}


void Monitors::init()
{
    foreach ( const QString connection , ConnectionManager::connectionNames() )
    {
        QSqlDatabase db = QSqlDatabase::database ( connection ) ;
        hostList.append( QPair<QString,QString>(db.hostName(), connection ));
        QSqlQuery query(db);

        query.exec ( "SELECT Value FROM Config WHERE Name='ZM_PATH_ZMS';" );
        //if we don't found this...game over
        if ( !query.next() ) {
            qDebug("Can't find the zm_path_zms config in database so we can't continue");
            return;
        }

        QString zms = query.value(0).toString();

        Auth auth ( connection );
        bool append = false;

        if ( auth.isAuthNeeded() )
        {
            if ( ! auth.isAuth() )
            {
                AuthDialog d ( &auth );
                d.setHostName( db.hostName() );
                int r = d.exec();
                if ( r == QDialog::Accepted )
                    append = true;
            }
            else append = true;
        }

        Stream::StreamMode streamMode = Stream::VIDEO; //just in case we don't found the name

        //this check for old and new name of stream types config in database
        query.exec( "SELECT Value FROM Config WHERE Name IN ('ZM_STREAM_METHOD', 'ZM_WEB_H_STREAM_METHOD');" );
        if ( query.next() ) {
            QString method = query.value ( 0 ).toString();
            streamMode = method == "jpeg" ? Stream::JPEG : Stream::VIDEO;
        }

        /*int count = 0;*/
        query.exec("SELECT Id, Name FROM Monitors;" );
        while ( query.next() )
        {
            /*count += 1;
            if (count > 1) break;*/
            CameraWidget * camera = new CameraWidget ( connection );
            uint id = query.value(0).toUInt();
            QString name = query.value(1).toString();

            camera->setWindowTitle ( name + tr( " at " ) + db.hostName() );
            camera->setName ( name );
            Stream * stream = camera->stream();

            stream->setHost ( db.hostName() , ConnectionManager::connectionWebPort( connection ) );
            stream->setMonitor ( id );
            stream->setMode ( streamMode );
            stream->setZMStreamServer ( zms );

            m_cameras.append ( camera );
            camera->setAutoAdjustImage ( true );
            if ( append )
                stream->appendZMSString ( auth.zmsString() );
            camera->startCamera();
        }
    }
}

int Monitors::count()
{
    return m_cameras.count();
}

QList<QPair< QString,QString> > Monitors::hosts() {
    return hostList;
}

void Monitors::updateMonitors()
{
    m_cameras.clear();
    hostList.clear();
    init();
}

Monitors::~Monitors()
{}




