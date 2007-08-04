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
#include "multicameraview.h"

#include "camerawidget.h"
#include "camerawidgettoolbar.h"
#include "stream.h"
#include "config.h"

#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>

MultiCameraView::MultiCameraView(QWidget * parent)
:QWidget( parent ),m_useTabs( false )
{
    init();
}

void MultiCameraView::init(){
    //m_cameras = new QList<CameraWidget *>;
}

void MultiCameraView::setUseTabs ( bool b ){
    m_useTabs = b;
}

void MultiCameraView::setStream( QList <Stream * > stream ){
    //m_cameras = cameras;
    QList <CameraWidget * > copy;
    for ( int i = 0 ; i < stream.size(); i++ ){
            copy.insert( i , new CameraWidget ( NULL, this ) );
            copy.at( i )->toolBar()->hide();
            copy.at( i )->setStream( stream.at(i));
            copy.at( i )->setConfigActionState(false);
            copy.at( i )->startCamera();
            
    }
    if (!m_useTabs) 
   layoutCameras ( copy );
   else layoutCamerasWithTabs ( copy );
}

void MultiCameraView::layoutCameras( QList <CameraWidget * > m_cameras ){
    QPushButton * quit = new QPushButton ( this );
    quit->setText(_("&Close View"));
    connect ( quit , SIGNAL(clicked()), this , SLOT(close() ));
    QVBoxLayout *parentLayout = new QVBoxLayout ( this );
    parentLayout->addWidget( quit );
    QGridLayout * l = new QGridLayout (  );
    parentLayout->addLayout( l );
    int s = m_cameras.size();
    int column = s / 2;
    if ( column < 1 ) column = 1;
        //evitar deformaciones con 2 o 3 camaras
        else if ( s  == 3  || s == 2 ) column = 2;

    int i =0;
    int row =0;
    int alternateRowCounter = 0;
    for (  ;i< s; i ++){
        for ( int j = 0 ; j < column && i < s ; j ++ ){
            l->addWidget ( m_cameras.at( i ) ,row , j  ,1,1);
            m_cameras.at( i )->setAutoAdjustImage(true);
            if ( j+1 < column )
                    i++;
            alternateRowCounter++;
        }

      row++;
     }
    int j=0;
    if ( alternateRowCounter < s ){
        for (  ;alternateRowCounter< s; alternateRowCounter ++){
            l->addWidget ( m_cameras.at( alternateRowCounter ) , row , j  ,1,1);
            m_cameras.at( alternateRowCounter )->setAutoAdjustImage(true);
            j++;
        }
    }
}


void MultiCameraView::layoutCamerasWithTabs ( QList <CameraWidget * > m_cameras , int camerasPerPage){
  
   QPushButton * quit = new QPushButton ( this );
    quit->setText(_("&Close View"));
    connect ( quit , SIGNAL(clicked()), this , SLOT(close() ));
    QVBoxLayout *parentLayout = new QVBoxLayout ( this );
    parentLayout->addWidget( quit );
    QTabWidget * cameraContainer = new QTabWidget ( this );
    parentLayout->addWidget(  cameraContainer );

    int s = m_cameras.size();
    int column = 2;
    if ( column < 1 ) column = 1;
        //evitar deformaciones con 2 o 3 camaras
        else if ( s  == 3  || s == 2 ) column = 2;

    int i =0;
    int row =0;
    bool newTab = true;
    QGridLayout * l = 0;
    QWidget * w = 0;
    int count = 0;
    int viewcount = 1;
    for (  ;i< s; i ++){
    if ( count == camerasPerPage - 1 || count >= camerasPerPage  || i == 0 ){
                    newTab =  true;
                    count = 0;
        }
         else { qDebug(qPrintable(QString::number( count )));
                newTab = false;
                }

         if ( newTab  ){
                    w = new QWidget;
                     l = new QGridLayout (  w );
                    w->setLayout( l );
                    cameraContainer->addTab( w , _("View ") + QString::number(  viewcount ) ); 
                     viewcount++;
            }

    
        for ( int j = 0 ; j < column && i < s ; j ++ ){
            count++;
             qDebug(qPrintable(QString::number( count )));
            m_cameras.at( i )->setAutoAdjustImage(true);
            l->addWidget ( m_cameras.at( i ) ,row , j  ,1,1);
            
            if ( j+1 < column )
                    i++;
        }

      row++;
     }
    }


void MultiCameraView::close(){
    //for ( int i = 0 ;i< m_cameras.size() ; i ++)
      //      m_cameras.at( i )->setAutoAdjustImage(false);
    QWidget::close();
}

MultiCameraView::~MultiCameraView()
{
//delete m_cameras;
}
#include "multicameraview.moc"
