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
#include "authdialog.h"

#include <QMessageBox>
AuthDialog::AuthDialog( Auth * auth ,  QWidget * parent )
:QDialog ( parent )
{
    m_auth = auth;
    setupUi( this );
    connect (okButton , SIGNAL(clicked() ) , this , SLOT(checkAuth()) );
    leUser->setFocus();
    setWindowTitle( tr("Auth") );
}

void AuthDialog::setHostName( const QString & host ){
    m_infoLabel->setText( tr("Monitors at <b>%1</b> need auth.").arg( host ) );
}

void AuthDialog::checkAuth(){
    bool b = m_auth->userLogin( leUser->text() , lePassword->text() );
    if ( b )
        done( QDialog::Accepted );
    else QMessageBox::critical( this , tr("Bad Login") , tr("User Name or Password are wrong!"));
}

AuthDialog::~AuthDialog()
{
}




