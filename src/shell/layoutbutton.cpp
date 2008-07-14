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

#include "layoutbutton.h"


LayoutButton::LayoutButton( int columns, QWidget * parent )
:QToolButton(parent),m_columns( columns ){
    m_layoutButtonType = ColumnLayout;
    if ( columns > 0 ){
        //search icons in :/shellicons/Grid[columns] resource
        setIcon( QIcon(QString(":/shellicons/Grid")+QString::number( columns ) )) ;
        setToolTip( tr("%1 Columns Layout" ).arg(QString::number( columns ) ) );
    }
    setCheckable( true );
    connect( this, SIGNAL (clicked () ), this, SLOT( changeLayout() )  );
}
int LayoutButton::columns(){ 
    return m_columns;
}

void LayoutButton::setButtonType( const LayoutButtonType &type ){
    m_layoutButtonType = type;
    if ( m_layoutButtonType != ColumnLayout ){
        disconnect( this, SIGNAL (clicked () ), this, SLOT( changeLayout() )  );
    }
}

void LayoutButton::setChecked( int column ){
    QToolButton::setChecked( column == m_columns );
}
void LayoutButton::changeLayout(){
    emit( clicked( m_columns ) );
}

#include "layoutbutton.moc"
