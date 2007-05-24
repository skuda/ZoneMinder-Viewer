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
#include "cameraadddialog.h"

CameraAddDialog::CameraAddDialog(QWidget * parent )
:QDialog( parent )
{
    setupUi( this );
}

void CameraAddDialog::setValues( const QString &name, const QString &host , const QString &port,const QString &monitor, const QString &zms ){
    m_name->setText( name );
    m_host->setText( host );
    m_port->setText(port);
    m_monitor->setText(monitor);
    m_zms->setText(zms);
}
void CameraAddDialog::completeDefaults(){
    m_host->setText( "localhost");
    m_port->setText("80");
    m_monitor->setText("1");
    m_zms->setText("/cgi-bin/nph-zms");
}


CameraAddDialog::~CameraAddDialog()
{
}


