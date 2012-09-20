/***************************************************************************
 *   Copyright (C) 2007 by                                                 *
 *   Leandro Emanuel López  lopezlean@gmail.com                            *
 *   Dardo Sordi Bogado dardosordi@gmail.com                               *
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
#include "stream.h"
#include "qmultipartreader.h"
#include "abstractplayer.h"
#include "vlcplayer.h"
#include "mjpegplayer.h"
#include "framewidget.h"

#include <QString>
#include <QPixmap>
#include <QTimer>
#include <QUrl>

class Stream::Private
{
    public:
        Private ( QObject * parent )
        {
            timer = new QTimer( parent );
            player = NULL;
            mode = "jpeg";
            host = "localhost";
            port = 80;
            monitor = 1;
            maxFps = 5;
            type = Monitor;
            scale = 100;
            bitrate = 100000;
            zms ="/cgi-bin/nph-zms";
        }
        ~Private(){
            if (player)
                delete player;
	    }

        QString mode;
        QString host;
        quint16 port;
        quint16 monitor;
        quint16 event;
        quint16 scale;
        quint32 bitrate;
        quint16 maxFps;
        QString zms;
        StreamType type;
        QTimer * timer;
        QString appendString;
        AbstractPlayer * player;
};

Stream::Stream ( QObject * parent )
        :QObject ( parent ),d ( new Private( this ) )
{
}

void Stream::setHost ( const QString & host, quint16 port )
{
    d->host = host;
    d->port = port;
}

void Stream::setMode ( const StreamMode &mode )
{
    if ( mode == JPEG )
    {
        d->mode = "jpeg";
        d->player = new MjpegPlayer(this);
        connect(d->player, SIGNAL(frameReady(QPixmap*)), SIGNAL(frameReady(QPixmap*)));

    } else {
        d->mode = "mpeg";
        d->player = new VlcPlayer(this);
    }

    connect(d->player, SIGNAL(done(QString)), SIGNAL(done(QString)));

    d->timer->start( 3000 );
    connect( d->timer, SIGNAL(timeout()), d->player, SLOT(checkConnection()) );
}

void Stream::setMonitor ( quint16 monitor )
{
    d->monitor = monitor;
}

void Stream::setStreamType ( const StreamType & t )
{
    d->type = t;
}

void Stream::setScale ( quint16 scale )
{
    d->scale = scale;
}

void Stream::setMaxFps(quint16 maxFps)
{
    d->maxFps = maxFps;
}

void Stream::setBitRate (quint32 bitrate )
{
    d->bitrate = bitrate;
}

void Stream::setZMStreamServer ( const QString &zms )
{
    d->zms = zms;
}

void Stream::setEvent ( quint16 event ){
    d->event = event;
}

void Stream::start(FrameWidget *frameWidget)
{
    QString const complete_url = completeUrl();
    qDebug(qPrintable(complete_url));

    d->player->setFrameWidget(frameWidget);
    d->player->start(complete_url);
    setStatus( Stream::Playing );
}

QString const Stream::completeUrl()
{
    QString connection;

    if (d->mode == "jpeg") {

        if (d->type == Monitor) {
            connection = QString("%1?mode=%2&monitor=%3&scale=%4&bitrate=%5")
                    .arg(d->zms).arg(d->mode).arg(d->monitor).arg(d->scale).arg(d->bitrate);
        }
        else if (d->type == Event) {
            connection = QString("%1?source=event&mode=%2&frame=1&event=%3&scale=%4&bitrate=%5")
                    .arg(d->zms).arg(d->mode).arg(d->event).arg(d->scale).arg(d->bitrate);
        }
    } else {
        if (d->type == Monitor) {
            connection = QString("%1?mode=%2&monitor=%3&scale=%4&bitrate=%5&maxpfs=%6&format=asf")
                    .arg(d->zms).arg(d->mode).arg(d->monitor).arg(d->scale).arg(d->bitrate).arg(d->maxFps);
        }
        else if (d->type == Event) {
            connection = QString("%1?source=event&mode=%2&frame=1&event=%3&scale=%4&bitrate=%5&maxpfs=%6&format=asf")
                    .arg(d->zms).arg(d->mode).arg(d->event).arg(d->scale).arg(d->bitrate).arg(d->maxFps);
        }
    }

    if (d->appendString.size() > 0)
        connection.append("&"+d->appendString);

    return QString("http://%1%2").arg(d->host).arg(connection);
}

void Stream::stop()
{
    setStatus( Stream::Stopped );
    d->player->stop();
}

QString Stream::host() const
{
    return d->host;
}

quint16 Stream::port() const
{
    return d->port;
}

QString Stream::mode() const
{
    return d->mode;
}

quint16 Stream::monitor() const
{
    return d->monitor;
}

quint32 Stream::bitrate() const
{
    return d->bitrate;
}

quint16 Stream::scale() const
{
    return d->scale;
}

quint16 Stream::maxFps() const
{
    return d->maxFps;
}

QString Stream::zmStreamServer() const
{
    return d->zms;
}

Stream::Status Stream::status() const
{
    return d->player->status();
}

void Stream::appendZMSString( const QString & s ){
   d->appendString = s;
}

QString Stream::ZMSStringAppended( ) const{
    return d->appendString;
}

void Stream::setStatus(Status status )
{
    d->player->setStatus(status);
    emit( statusChanged(status) );
}

Stream::~Stream()
{
    delete d;
}
