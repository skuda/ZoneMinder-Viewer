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
#ifndef CAJAJPERCONFIGDIALOGPAGE_H
#define CAJAJPERCONFIGDIALOGPAGE_H

#include <QWidget>


class ConfigDialogPage : public QWidget{
    Q_OBJECT
public:
    ConfigDialogPage ( QWidget * parent = 0 );
    void setTitle( const QString & title );
    QString title() const;
    void setSubTitle( const QString &subtitle );
    QString subTitle()const;
    void setIcon( const QIcon & icon );
    QIcon icon()const;

    virtual bool dialogAccepted() {return true;}
    ~ConfigDialogPage();

private:
    class Private;
    Private *d;
};

#endif
