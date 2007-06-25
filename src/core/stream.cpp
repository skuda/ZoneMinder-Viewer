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

#include <QHttp>
#include <QString>
#include <QPixmap>


class Stream::Private
{
    public:
        Private ( QObject * parent )
        {
            http = new QHttp( parent );
            http->setUser("admin","admin");
            mode = "jpeg";
            host = "localhost";
            port = 80;
            monitor = 1;
            type = Monitor;
            scale = 100;
            bitrate = 2;
            zms ="/cgi-bin/nph-zms";
            current_frame = new QPixmap;
        }
        ~Private(){
            delete current_frame;
	    }
        QHttp * http;
        QString mode;
        QString host;
        quint16 port;
        quint16 monitor;
        quint16 event;
        quint16 scale;
        quint16 bitrate;
        QString zms;
        StreamType type;
        QPixmap * current_frame;
        QString appendString;
};

Stream::Stream ( QObject * parent )
        :QObject ( parent ),d ( new Private( this ) )
{
}

void Stream::setHost ( const QString & host, quint16 port )
{
    d->host = host;
    d->port = port;
    d->http->setHost ( host , port );
}

void Stream::setMode ( const StreamMode &mode )
{
    if ( mode != JPEG )
    {
        qDebug ( "Stream:setMode(): Not implemented yet!" );
        d->mode = "Invalid";
    }
    d->mode = "jpeg";
}

void Stream::setMonitor ( quint16 monitor )
{
    d->monitor = monitor;
}

void Stream::setStreamType ( const StreamType & t ){
    d->type = t;
}
void Stream::setScale ( quint16 scale )
{
    d->scale = scale;
}
void Stream::setBitRate ( quint16 bitrate )
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

void Stream::read ( const QHttpResponseHeader &header )
{
    Q_UNUSED ( header );
    QByteArray array = d->http->readAll();
    QByteArray * tmp = new QByteArray;
    int c = 0;
    const static QByteArray h ( "--ZoneMinderFrame\r\n" );
    const static QByteArray e ( "\r\n\r\n" );
    const static QByteArray rn ( "\r\n" );

    int index = -1;
    int end = -1;

    if ( ( index = array.lastIndexOf ( h, index ) ) != -1 )
    {

        if ( ( end = array.indexOf ( e, index ) ) != -1 )
        {
            int len = -1;
            len = QString ( array.mid ( index+19+16, 6 ) ).toInt();

            tmp->push_back ( array.mid ( index+69, len ) );
            image ( tmp );

        }



    }
    delete tmp;


    c++;
}


bool Stream::image ( QByteArray* array )
{
    if ( d->current_frame->loadFromData ( *array ) )
    {
        array->clear();
        emit ( frameReady ( d->current_frame ) );
        return true;
    }
    qDebug ( "Stream::image: NOT ready" );
    return false;
}



void Stream::start()
{
    QString connection;
    if (d->type == Monitor )
        connection = QString ( "%1?mode=%2&monitor=%3&scale=%4&bitrate=%5" ).arg ( d->zms ).arg ( d->mode ).arg ( d->monitor ).arg ( d->scale ).arg ( d->bitrate );
    else if ( d->type == Event )
            connection = QString ( "%1?mode=%2&frame=1&event=%3&scale=%4&bitrate=%5" ).arg ( d->zms ).arg ( d->mode ).arg ( d->event ).arg ( d->scale ).arg ( d->bitrate);
    if ( !d->appendString.isNull() && d->appendString.size() > 0 )
            connection.append("&"+d->appendString);
    qDebug ( qPrintable ( connection ) );
    d->http->get ( connection );
    connect ( d->http, SIGNAL ( readyRead ( const QHttpResponseHeader& ) ), this, SLOT ( read ( const QHttpResponseHeader & ) ) );
}

void Stream::stop(){
    d->http->abort ();
    disconnect ( d->http, SIGNAL ( readyRead ( const QHttpResponseHeader& ) ), this, SLOT ( read ( const   QHttpResponseHeader & ) ) );
    
}
void Stream::restart(){
    stop();
    start();
}

//

QString Stream::host() const
{
    return d->host;
};
quint16 Stream::port() const
{
    return d->port;
};
QString Stream::mode() const
{
    return d->mode;
};
quint16 Stream::monitor() const
{
    return d->monitor;
};
quint16 Stream::bitrate() const
{
    return d->bitrate;
};
quint16 Stream::scale() const
{
    return d->scale;
};
QString Stream::zmStreamServer() const
{
    return d->zms;
};

void Stream::appendZMSString( const QString & s ){
   d->appendString = s;
}

QString Stream::ZMSStringAppended( ) const{
    return d->appendString;
}
Stream::~Stream()
{
    delete d;
}




#include "stream.moc"
