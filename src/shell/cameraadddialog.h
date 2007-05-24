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
#ifndef CAMERAADDDIALOG_H
#define CAMERAADDDIALOG_H

#include <QDialog>
#include "ui_cameraadddialog.h"

class CameraAddDialog: public QDialog, public Ui::CameraAddDialog{
public:
    CameraAddDialog( QWidget * parent );
    void setValues( const QString &name ,const QString &host , const QString &port,const    QString &monitor, const QString &zms );
    void completeDefaults();
    ~CameraAddDialog();

};

#endif
