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
#ifndef STYLEDIALOG_H
#define STYLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
/**
	@author Leandro Emanuel Lopez <lopezlean@gmail.com>
*/
class Style;
/**
 TODO: this need more love
*/
class EntryButton: public QPushButton{
  Q_OBJECT
  public:
    EntryButton( const Style &s , const QString &text,QWidget * parent = 0 );
    Style *style;
};

class EntryView : public QLabel{
Q_OBJECT
 public:
    EntryView( QWidget * parent = 0 );
    void setStyle( const Style &s );
    void setEntryViewStyle();
    static QPalette defaultPalette();
 private Q_SLOTS:
    void urlSelected( const QString &link);
};



class StyleDialog : public QDialog
{
Q_OBJECT
public:
    StyleDialog( QWidget * parent = 0 );

    ~StyleDialog();
private Q_SLOTS:
    void urlSelected(const QString&);
    void applyStyle();
private:
 void init();
 class Private;
 Private *d;


};

#endif
