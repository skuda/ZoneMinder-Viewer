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
#ifndef STREAM_H
#define STREAM_H

#include <QObject>
class QString;
class QHttpResponseHeader;
class QPixmap;
class QByteArray;

class Stream: public QObject{
Q_OBJECT
public:
    enum StreamMode{ JPEG, VIDEO/** TODO: VIDEO */ };
    enum StreamType{ Monitor, Event };
    Stream( QObject * parent = 0 );
    ~Stream();
    
    QString host()const;
    quint16 port()const;
    QString mode()const;
    quint16 monitor()const;
    quint16 bitrate()const;
    quint16 scale()const;
    QString zmStreamServer()const;

    /** start Stream*/
    void start();
    void stop();
    void restart();

    void appendZMSString( const QString & s );
    QString ZMSStringAppended( ) const;
public Q_SLOTS:
    void setHost( const QString & host , quint16 port = 80 );
    void setMode ( const StreamMode &mode );
    void setMonitor ( quint16 monitor );
    void setStreamType ( const StreamType & t );
    void setEvent ( quint16 event );
    void setBitRate ( quint16 bitrate );
    void setScale ( quint16 scale );
    void setZMStreamServer ( const QString &zms = "/cgi-bin/nph-zms" );

Q_SIGNALS:
    /**
        Esta señal es emitida por cada vez que se produce una imagen y
        debe de ser atrapada por algun objeto externo.
    */
    void frameReady ( QPixmap * current_frame );
private Q_SLOTS:
    void read (const QHttpResponseHeader &header );
    bool image ( const QByteArray &array );
private:

    //private class
    class Private;
    Private *d;

};

#endif
