/***************************************************************************
*   Copyright (C) 2008 by Leandro Emanuel López                           *
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

#include "cameralayout.h"
#include "blackwidget.h"


class CameraLayout::Private{
public:
    int rows;
    int columns;
    int columnCount;
    int rowCount;
    bool hasSpace;
    QSize cellsSize;
};

CameraLayout::CameraLayout( int rows, int columns )
:QGridLayout(), d ( new Private )
{
    d->rows = rows;
    d->columns = columns;
    d->columnCount = 0;
    d->rowCount = 0;
    d->hasSpace = true;
    setSizeConstraint( QLayout::SetMaximumSize );
    setSpacing( 2 );
    setMargin( 1 );
    setContentsMargins( 0,0,0,0 );
    fill();
}
void CameraLayout::setGrid( int rows, int columns ){
    //clear all widgets;
    setRows( rows );
    setColumns ( columns );
    reset();

}
void CameraLayout::setRows( int rows ){
    d->rows = rows;
}
void CameraLayout::setColumns( int columns ){
    d->columns = columns;
}

void CameraLayout::addWidget ( QWidget * widget ){
    if ( !checkLayoutDimensions ( ) ){
         qDebug("CameraLayout::addWidget: Not Enough Space" );
         return;
    }
    QLayoutItem *item = itemAtPosition( d->rowCount, d->columnCount );
    QSize size = item->widget()->size();
    if ( item ){
        removeItem( item );
        delete item->widget();
        //delete item;
    }
    widget->setFixedSize( size );
    QGridLayout::addWidget ( widget, d->rowCount , d->columnCount );
    d->columnCount++;
    //setColumnStretch( d->columnCount , size.width() );
    if ( d->columnCount >= d->columns ) {
            d->rowCount++;
            d->columnCount = 0;
    }
}

bool CameraLayout::hasSpace(){
    checkLayoutDimensions();
    return d->hasSpace;
}

bool CameraLayout::checkLayoutDimensions(){
    if ( d->rowCount > ( d->rows -1 ) ){
        d->hasSpace = false;
        return d->hasSpace;
    }
    else if ( d->rowCount == ( d->rows -1 )  ){
        if ( d->columnCount > ( d->columns -1 ) ){
            d->hasSpace = false;
            return d->hasSpace;
            }
        } else {
            d->hasSpace = true;
            return d->hasSpace;
    }

    d->hasSpace = true;
    return d->hasSpace;
}

QSize CameraLayout::cellsSizeHint(){
    calculateCellsSize();
    return d->cellsSize;
}

int CameraLayout::maxElements()const{
    return (d->rows * d->columns);
}

void CameraLayout::calculateCellsSize( ){
    QSize parentSize = parentWidget()->size();
    int wSize = (parentSize.width() / d->columns ) - horizontalSpacing();
    int hSize = (parentSize.height() / d->rows) - verticalSpacing();
    d->cellsSize = QSize( wSize, hSize );
}

void CameraLayout::fill(){
    for ( int r = 0 ; r < d->rows ; r++ ){
        for ( int c = 0 ; c < d->columns ; c++ ){
            BlackWidget *widget = new BlackWidget();
            QGridLayout::addWidget ( widget, r , c );
        }
    }
}

QSize CameraLayout::sizeHint() const{
     if (parentWidget())
        return parentWidget()->size();
     return QGridLayout::sizeHint();
}

void CameraLayout::clear(){
    for (int r = 0; r < rowCount(); ++r ){
        for (int c = 0; c < columnCount(); ++c ){
                QLayoutItem *item = itemAtPosition( r, c );
                if ( item ){
                        removeItem( item );
                        if ( item->widget() ){
                            BlackWidget *bw = qobject_cast < BlackWidget *> ( item->widget() );
                            if ( bw ) delete bw;
                            //else is camera o similar
                            else item->widget()->setVisible( false );
                        }
                }
            }
        }
}

void CameraLayout::updateLayout(){
    calculateCellsSize();
    for (int r = 0; r < rowCount(); ++r ){
        for (int c = 0; c < columnCount(); ++c ){
                QLayoutItem *item = itemAtPosition( r, c );
                if ( item ){
                        if ( item->widget() ){
                            item->widget()->setFixedSize( d->cellsSize );
                            setColumnStretch( c, d->cellsSize.width() );
                            setRowStretch( c, d->cellsSize.height() );
                        }
                }
            }
        }
}

void CameraLayout::reset(){
    setSpacing( 2 );
    setMargin( 1 );
    clear();
    calculateCellsSize();
    checkLayoutDimensions();
    fill();
    d->rowCount = 0;
    d->columnCount = 0;
    d->hasSpace = true;
}

CameraLayout::~CameraLayout()
{
    delete d;
}


