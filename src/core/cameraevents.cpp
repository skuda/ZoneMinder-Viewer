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
#include "cameraevents.h"
#include "camerawidget.h"
#include "stream.h"


#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>

CameraEvents::CameraEvents ( int camId, const QString & connectionName , QWidget * parent )
        :QDialog ( parent )
{
    m_cameraId = camId;
    m_connectionName = connectionName;
    init();
}

void CameraEvents::init()
{
    QVBoxLayout * layout = new QVBoxLayout ( this );
    m_model = new QSqlTableModel ( this , QSqlDatabase::database ( m_connectionName ) );
    m_model->setTable ( "Events" );
    m_model->setFilter ( "MonitorId = " + QString::number ( m_cameraId ) );
    m_model->setEditStrategy ( QSqlTableModel::OnManualSubmit );
    m_model->select();
    /* TODO
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Monitor"));
    */
    m_view = new QTableView ( this );
    m_view->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    m_view->setSelectionBehavior ( QAbstractItemView::SelectRows );
    m_view->setAlternatingRowColors ( true );
    m_view->setModel ( m_model );
    layout->addWidget ( m_view );

    connect ( m_view , SIGNAL ( doubleClicked ( QModelIndex ) ), this , SLOT ( showEvent ( QModelIndex ) ) );
}

void CameraEvents::showEvent ( const QModelIndex & index )
{
    int eventId = m_model->data ( m_model->index ( index.row(), 0 ) ).toInt();
    QSqlDatabase db = QSqlDatabase::database ( m_connectionName );
    QSqlQuery query = db.exec ( "SELECT Value from Config where Name='ZM_PATH_ZMS'" );
    query.next();
    QString zms = query.value ( 0 ).toString();
    query.clear();
    query = db.exec ( "SELECT * from Monitors where Id = " + m_model->data ( m_model->index ( index.row(), 1 ) ).toString() );
    query.next();

    CameraWidget * camera = new CameraWidget ( );
    camera->setWindowTitle ( m_model->data ( m_model->index ( index.row(), 2 ) ).toString() );
    camera->stream()->setHost ( db.hostName() ,query.value ( query.record().indexOf ( "Port" ) ).toInt() );
    camera->stream()->setStreamType ( Stream::Event );
    camera->stream()->setEvent ( eventId );
    camera->stream()->setZMStreamServer ( zms );
    camera->setAutoAdjustImage ( true );
    camera->startCamera();
    query.clear();
    camera->show();



}
CameraEvents::~CameraEvents()
{}

#include "cameraevents.moc"

