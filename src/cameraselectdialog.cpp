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
#include "cameraselectdialog.h"

#include <QVBoxLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>
#include <QFrame>

CameraSelectDialog::CameraSelectDialog( QWidget * parent )
:QDialog ( parent )
{
 
    init();
}

void CameraSelectDialog::init(){
    setWindowTitle ("Seleccionar camaras");
}

void CameraSelectDialog::setNames (  const QStringList &names ){
        QVBoxLayout *l = new QVBoxLayout( this );
        for ( int i = 0 ; i < names.size() ; i++ ){
		 QCheckBox * insertCheckBox = new QCheckBox(names.at(i),this );
		 insertCheckBox->setChecked( true );
                 m_checkBoxes.insert(i,insertCheckBox );
                 l->addWidget( m_checkBoxes.at(i));
                }

    QFrame * line = new QFrame(this );
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(170, 120, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    l->addWidget ( line );

    m_groupByView = new QCheckBox ("Agrupar por vistas", this );
    l->addWidget(m_groupByView);
    QDialogButtonBox * bx = new QDialogButtonBox(this);
    QPushButton * ok = bx->addButton(QDialogButtonBox::Ok);
    QPushButton * cancel = bx->addButton(QDialogButtonBox::Cancel);
    connect ( ok ,SIGNAL(clicked()),this ,SLOT(selectCameras()));
    connect (cancel,SIGNAL(clicked()), this, SLOT(reject()));
    l->addWidget( bx );

}

QStringList CameraSelectDialog::names(){
    QStringList names;
    for( int i = 0 ; i<m_checkBoxes.size();i++){
        if ( m_checkBoxes.at( i )->isChecked() )
                names << m_checkBoxes.at( i )->text();
    }
    return names;
}

bool CameraSelectDialog::groupViewView() const {
   return  m_groupByView->isChecked();
}

void CameraSelectDialog::selectCameras(){
    if (names().size() == 0 ){
        QMessageBox::warning(this, "Seleccionar campos", "Debes seleccionar al menos una camara para la vista" );
        return;
    }
    emit(accept ());
}

CameraSelectDialog::~CameraSelectDialog()
{

}


