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
#include "framewidget.h"

#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>

class FrameWidget::Private{
public:
    QPixmap pixmap;
    Stream::Status status;
    QString id;
    QString message;
    Qt::AspectRatioMode aspectRatioMode;
    Qt::Alignment numbersPosition;
    Qt::Alignment eventTextAlignment;
    bool showNumbers;
    Qt::TransformationMode transformationMode;
    bool showEventsText;
};

FrameWidget::FrameWidget( QWidget * parent )
:QWidget( parent ), d ( new Private )
{
    init();
}

void FrameWidget::init(){
    setAttribute(Qt::WA_NoSystemBackground);
    d->status = Stream::None;
    d->aspectRatioMode = Qt::KeepAspectRatio;
    d->showNumbers = true;
    d->numbersPosition = Qt::AlignRight | Qt::AlignTop;
    d->eventTextAlignment = Qt::AlignBottom | Qt::AlignRight;
    d->transformationMode = Qt::FastTransformation;
    d->showEventsText = false;
}

void FrameWidget::setPixmap( const QPixmap &pixmap ){
    if ( ! pixmap.isNull() )
        d->pixmap = pixmap.scaled(size(), d->aspectRatioMode, d->transformationMode );
    else d->pixmap = pixmap;
    update( contentsRect() );
}

void FrameWidget::setAspectRatioMode( const Qt::AspectRatioMode & mode ){
    d->aspectRatioMode = mode;
    update();
}
void FrameWidget::setShowNumbers( bool b ){
    d->showNumbers = b;
    update();
}
void FrameWidget::setNumbersPosition( const Qt::Alignment & alignment ) {
    d->numbersPosition = alignment;
    if ( alignment == ( Qt::AlignBottom | Qt::AlignRight ) )
        d->eventTextAlignment = Qt::AlignTop | Qt::AlignRight;
    else d->eventTextAlignment = Qt::AlignBottom | Qt::AlignRight;
    update();
}

void FrameWidget::setTransformationMode( const Qt::TransformationMode &mode ){
    d->transformationMode = mode;
}
void FrameWidget::setHasNewEvents( bool b){
    d->showEventsText = b;
}
void FrameWidget::setStatus( const Stream::Status & status , const QString & message ){
    d->status = status;
    setMessage( message );
    update();
}

void FrameWidget::setFrameId( const QString &id ){
    d->id = id;
}

bool FrameWidget::showNumbers()const{
    return d->showNumbers;
}
Qt::Alignment FrameWidget::numbersPosition()const{
    return d->numbersPosition;
}
Qt::TransformationMode FrameWidget::transformationMode() const{
    return d->transformationMode;
}
Qt::AspectRatioMode FrameWidget::aspectRatioMode()const{
    return d->aspectRatioMode;
}

Stream::Status FrameWidget::status() const{
    return d->status;
}
QPixmap FrameWidget::currentPixmap() const{
    return d->pixmap;
}
void FrameWidget::updateSize(){
    if ( !d->pixmap.isNull() ){
        d->pixmap = d->pixmap.scaled(size());
    }
    update( contentsRect() );
}
void FrameWidget::setMessage( const QString & msg ){
    d->message = msg;
}
void FrameWidget::paintEvent ( QPaintEvent * event ){
    QPainter painter( this );
    painter.setCompositionMode( QPainter::CompositionMode_Source );
    painter.setRenderHint( QPainter::Antialiasing );
    QRect exposedRect = painter.matrix().inverted()
                     .mapRect(event->rect())
                     .adjusted(-1, -1, 1, 1);
    QRect target = rect();

    if ( d->aspectRatioMode == Qt::KeepAspectRatio ){
        int left = qRound(( target.width() - d->pixmap.width()   ) / 2 );
        int top = qRound(( target.height() - d->pixmap.height()   ) / 2 );
        target.setTop( top );
        target.setLeft( left );
    }

    painter.drawPixmap( target, d->pixmap , exposedRect );
    if ( d->showNumbers )
        drawId( &painter );
    drawText( &painter );
    if ( d->showEventsText ) drawEventsText( &painter );
}

void FrameWidget::drawText( QPainter * painter ){
    painter->setPen( Qt::white );
    switch( d->status ){
        case Stream::Stopped:
            if ( d->message.isEmpty() ){
                painter->drawText(rect(),Qt::AlignCenter, tr("Stopped") );
            } else {
                painter->drawText(rect(),Qt::AlignCenter | Qt::TextWordWrap, d->message );
            }
            break;
        case Stream::Paused:
            painter->drawText(rect(),Qt::AlignCenter, tr("Paused") );
            break;
        case Stream::NoSignal:
            painter->drawText(rect(),Qt::AlignCenter, tr("No Signal") );
            break;
        case Stream::HTTPError:
            if ( d->message.isEmpty() ){
                painter->drawText(rect(),Qt::AlignCenter, tr("Connection Error") );
            } else {
                painter->drawText(rect(),Qt::AlignCenter | Qt::TextWordWrap, d->message );
            }
            break;
    }

}

void FrameWidget::drawEventsText ( QPainter * painter  ){
    painter->setPen( Qt::white );

    painter->drawText(rect(), d->eventTextAlignment , tr("New Events Detected") );
}

void FrameWidget::drawId( QPainter * painter ){
    /**
    TODO: make it configurable
    */
    painter->setPen( QColor("#FFFF00") );
    painter->drawText( rect() ,d->numbersPosition , d->id );
}

qreal FrameWidget::ratioX(){
    if ( size().width() > 0 && d->pixmap.size().width() )
        return (qreal)( (size().width() + 0.00 ) / d->pixmap.size().width() );
    else return 1;
}
qreal FrameWidget::ratioY(){
    if ( size().height() > 0 && d->pixmap.size().height() > 0 )
    return (qreal)( (size().height() + + 0.00 ) / d->pixmap.size().height() );
    else return 1;
}

FrameWidget::~FrameWidget()
{
    delete d;
}

#include "framewidget.moc"
