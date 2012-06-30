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
#include "connectionwidget.h"
#include "connectionmanager.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSqlDatabase>
#include <QVBoxLayout>
#include <QMenu>
#include <QMouseEvent>
#include <QSettings>

ConnectionWidget::ConnectionWidget ( QWidget * parent )
        :QDialog ( parent )
{
    init();
    setupActions();
}


void ConnectionWidget::init()
{
    setWindowTitle( tr("Session List") );
    m_table = new QTableWidget ( this );
    m_table->setContextMenuPolicy ( Qt::ActionsContextMenu );
    m_table->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    //connect ( m_table , SIGNAL ( cellClicked ( int , int ) ) , this , SLOT ( removeConnectionCheck ( int ,int ) ) );
    m_table->setSelectionBehavior ( QAbstractItemView::SelectRows );
    QVBoxLayout * v = new QVBoxLayout ( this );
    v->addWidget ( m_table );
    m_table->setColumnCount ( 6 );
    QStringList labels;
    int r=0;
    labels << tr( "Host" ) << tr( "Database" ) << tr( "User" ) << tr( "Driver" ) << tr( "Port" ) << tr( "State" );
    m_table->setHorizontalHeaderLabels ( labels );
    foreach ( const QString connection, ConnectionManager::connectionNames() )
    {
        QSqlDatabase db = QSqlDatabase::database ( connection ) ;
        m_table->insertRow ( r );
        m_table->setItem ( r , 0 , new QTableWidgetItem ( db.hostName() ) );
        m_table->setItem ( r , 1 , new QTableWidgetItem ( db.databaseName() ) );
        m_table->setItem ( r , 2 , new QTableWidgetItem ( db.userName() ) );
        m_table->setItem ( r , 3 , new QTableWidgetItem ( db.driverName() ) );
        m_table->setItem ( r , 4 , new QTableWidgetItem ( QString::number ( db.port() ) ) );
        m_table->setItem ( r , 5 , new QTableWidgetItem ( db.isOpenError() ? tr("Failed") : tr("Connected") ) ) ;
        r++;
    }
    resize ( 650 , 300 );

}

void ConnectionWidget::setupActions()
{
    QAction * actRemove = new QAction ( QIcon ( ":/icons/Remove" ),tr( "Remove" ), this );
    m_table->addAction ( actRemove );
    connect ( actRemove,SIGNAL ( triggered() ), this , SLOT ( removeConnection () ) );
}
void ConnectionWidget::removeConnection( )
{
    if ( ! m_table->currentItem() ) return;
    int row = m_table->currentItem()->row();
    if ( row < 0 ) return;
    //driver,host,database,user,port
    QString cn = m_table->item ( row , 3 )->text() + m_table->item ( row , 0 )->text() + m_table->item ( row , 1 )->text() +m_table->item ( row , 2 )->text() +m_table->item ( row , 4 )->text();
    QSqlDatabase::removeDatabase ( cn );
    m_table->removeRow ( row );
    QSettings s;
    s.beginGroup ( "Databases" );
    QStringList v = s.value ( "names" ).toStringList();
    v.removeAt(v.indexOf( cn ));
    s.setValue( "names" , v );
    s.endGroup();
    s.remove( cn );
    ConnectionManager::removeConnectionWebPort( cn );
    emit( needUpdate());

}

ConnectionWidget::~ConnectionWidget()
{}




