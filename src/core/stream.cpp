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

#include <QNetworkAccessManager>
#include <QNetworkReply>
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
            manager = new QNetworkAccessManager(parent);

            mode = "jpeg";
            host = "localhost";
            port = 80;
            monitor = 1;
            type = Monitor;
            scale = 100;
            bitrate = 2;
            reconnectCount = 0;
            zms ="/cgi-bin/nph-zms";
            current_frame = new QPixmap;
            multiPartReader = new QMultiPartReader("--ZoneMinderFrame", parent );
        }
        ~Private(){
            delete manager;
            delete current_frame;
            delete multiPartReader;
	    }
        QNetworkAccessManager * manager;
        QNetworkReply * reply;

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
        QTimer * timer;
        QString appendString;
        Status status;
        // used to check connection errors
        Status userStatus;
        int reconnectCount;
        bool finishedConnection;

        QMultiPartReader * multiPartReader;
};

Stream::Stream ( QObject * parent )
        :QObject ( parent ),d ( new Private( this ) )
{
    init();
}

void Stream::init(){
    d->status = Stream::None;
    d->timer->start( 3000 );
    connect ( d->timer , SIGNAL( timeout() ), this, SLOT( checkConnection() ) );
}

void Stream::setHost ( const QString & host, quint16 port )
{
    d->host = host;
    d->port = port;
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

void Stream::read()
{
    if (d->reply->bytesAvailable()) {
        QByteArray r = d->multiPartReader->read( d->reply->readAll() );
        if ( !r.isNull() )
            image( r );
    }
}

bool Stream::image ( const QByteArray &array )
{
    if ( d->current_frame->loadFromData ( array ) )
    {
        emit ( frameReady ( d->current_frame ) );
        return true;
    }
#ifdef DEBUG_PARSING
    qDebug ( "Stream::image: NOT ready" );
#endif
    return false;
}

void Stream::start()
{
    d->userStatus = Playing;
    d->finishedConnection = false;
    setStatus( Stream::Playing );
    QString connection;

    if (d->type == Monitor )
        connection = QString("%1?mode=%2&monitor=%3&scale=%4&bitrate=%5")
                     .arg(d->zms).arg(d->mode).arg(d->monitor).arg(d->scale).arg(d->bitrate);

    else if ( d->type == Event )
        connection = QString("%1?source=event&mode=%2&frame=1&event=%3&scale=%4&bitrate=%5")
                     .arg(d->zms).arg(d->mode).arg(d->event).arg(d->scale).arg(d->bitrate);

    if (d->appendString.size() > 0)
        connection.append("&"+d->appendString);

    QString complete_url = QString("http://%1%2").arg(d->host).arg(connection);
    qDebug(qPrintable(complete_url));

    d->reply = d->manager->get(QNetworkRequest(QUrl(complete_url)));
    connect( d->reply, SIGNAL(readyRead()), this, SLOT(read()) );
    connect( d->reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(readError(QNetworkReply::NetworkError)) );
    connect( d->reply, SIGNAL(finished()), this, SLOT(finishedConnection()) );
}

void Stream::stop()
{
    d->userStatus = Stopped;
    setStatus( Stream::Stopped );
    d->finishedConnection = true;
    disconnect( d->reply, SIGNAL(readyRead()), this, SLOT(read()) );
    disconnect( d->reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(readError(QNetworkReply::NetworkError)) );
    disconnect( d->reply, SIGNAL(finished()), this, SLOT(finishedConnection()) );
}

void Stream::restart(){
    stop();
    start();
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

quint16 Stream::bitrate() const
{
    return d->bitrate;
}

quint16 Stream::scale() const
{
    return d->scale;
}

QString Stream::zmStreamServer() const
{
    return d->zms;
}

Stream::Status Stream::status() const{
    return d->status;
}

void Stream::appendZMSString( const QString & s ){
   d->appendString = s;
}

QString Stream::ZMSStringAppended( ) const{
    return d->appendString;
}

void Stream::setStatus( const Status & status ){
    d->status = status;
    emit( statusChanged( d->status ) );
}

void Stream::readError ( QNetworkReply::NetworkError error ){
    if (error != QNetworkReply::NoError && d->userStatus != Stream::Stopped) {
        setStatus ( Stream::HTTPError );
        emit ( done( d->reply->errorString() ) );
    }
    else {
        if ( d->type == Event ){
            setStatus( Stream::Stopped );
            emit ( done( tr( "Event finished.") ) );
        } else {
            if ( d->userStatus != Stream::Stopped ){
                setStatus( Stream::NoSignal );
                emit ( done( tr( "Stopped by server. Press play to try again") ) );
            } else {
                setStatus( d->userStatus );
            }
        }
    }
}

void Stream::finishedConnection()
{
    d->finishedConnection = true;
}

void Stream::checkConnection() {
    if ( d->userStatus == Stream::Playing && d->finishedConnection ) {
        //try to connect
        restart();
    } else if ( d->reconnectCount != 0 && !d->finishedConnection ){
        d->reconnectCount = 0;
    }
}

Stream::~Stream()
{
    delete d;
}
