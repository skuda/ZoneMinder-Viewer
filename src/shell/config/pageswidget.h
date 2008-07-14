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
#ifndef SCONFIGDIALOG_H
#define SCONFIGDIALOG_H

#include <QWidget>
#include <QIcon>

class QStackedWidget;
class QListWidget;
class QListWidgetItem;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QFrame;
class QDialogButtonBox;

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/

class PagesWidget : public QWidget
{
    Q_OBJECT
public:
    PagesWidget( QWidget * parent = 0);
    void addWidget ( QWidget * widget , const QString & text, const QString & opttext = QString() , QIcon icon = QIcon(":/icons/PagesWidgetDefault") );
    ~PagesWidget();
    QList <QWidget * > allWidgets();
protected:
    QStackedWidget * mOptionWidget;
    QListWidget * mListWidget;
    QLabel * mCurrentOption;
    QLabel * mCurrentIcon;
    QLabel * mCurrentOptionText;

protected Q_SLOTS:
    virtual void changePage(QListWidgetItem *current, QListWidgetItem
                            *previous);

private:
    void init ();
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QFrame *frame;
    QHBoxLayout *hboxLayout1;
    QHBoxLayout *hboxLayout2;
    QList <QWidget *> mWidgets;
    //oops ;)
    friend class ConfigDialog;


};

#endif

