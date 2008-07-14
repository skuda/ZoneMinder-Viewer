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
#ifndef LANGUAGECONFIGWIDGET_H
#define LANGUAGECONFIGWIDGET_H

#include "configdialogpage.h"
#include "ui_languageconfigwidget.h"

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
class Translator;
class LanguageConfigWidget: public ConfigDialogPage, public Ui::LanguageConfigWidget{
Q_OBJECT
public:
    LanguageConfigWidget( QWidget * parent = 0 );
    bool dialogAccepted();
    ~LanguageConfigWidget();
private Q_SLOTS:
    void setLocaleFromComboBox( int index );
private:
    void initLanguageComboBox();
    Translator * m_translator;
    QString m_localeString;

};

#endif
