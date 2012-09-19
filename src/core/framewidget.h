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

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
#include <QWidget>

#include "stream.h"

class FrameWidget : public QWidget{
    Q_OBJECT

public:
    FrameWidget( QWidget * parent = 0 );
    void setStatus( const Stream::Status & status , const QString & message = QString() );
    void setFrameId( const QString &id );
    void setAspectRatioMode( const Qt::AspectRatioMode & mode );
    void setShowNumbers( bool b );
    void setNumbersPosition( const Qt::Alignment & alignment ) ;
    void setTransformationMode( const Qt::TransformationMode &mode );
    void setHasNewEvents( bool b);

    Stream::Status status() const;
    QPixmap currentPixmap() const;
    QString frameId() const;
    bool showNumbers() const;
    Qt::Alignment numbersPosition() const;
    Qt::TransformationMode transformationMode() const;
    Qt::AspectRatioMode aspectRatioMode() const;

    void updateSize();
    ~FrameWidget();

public Q_SLOTS:
    void setPixmap( const QPixmap &pixmap );
    void setMessage( const QString & msg );

protected:
    void paintEvent ( QPaintEvent * event );

private:
    void init();
    qreal ratioX();
    qreal ratioY();
    void drawText( QPainter * painter );
    void drawEventsText ( QPainter * painter  );

    void drawId( QPainter * painter );

    class Private;
    Private *d;

};

#endif
