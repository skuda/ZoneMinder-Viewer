/***************************************************************************
 *   Copyright (C) 2007 by Leandro Emanuel López   *
 *   lopezlean@gmail.com   *
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

#include "configdialog.h"
#include "configdialogpage.h"

#include "pageswidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QListWidget>


ConfigDialog::ConfigDialog( QWidget * parent )
 : QDialog( parent )
{
    init();
}

void ConfigDialog::init(){
    setWindowTitle( tr("Preferences") );
    QVBoxLayout * layout = new QVBoxLayout;
    m_pagesWidget = new PagesWidget( this );
    m_buttonBox = new QDialogButtonBox(this);
    m_buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    layout->addWidget( m_pagesWidget );
    layout->addWidget(m_buttonBox);
    connect( m_buttonBox, SIGNAL( accepted( ) ), this, SLOT( dialogAccepted( ) ) );
    connect( m_buttonBox, SIGNAL( rejected( ) ), this, SLOT(reject( ) ) );
    setLayout( layout );

    //m_pagesWidget->mListWidget->setSpacing(25);
    //m_pagesWidget->mListWidget->setFlow( QListWidget::TopToBottom );
    //m_pagesWidget->mListWidget->setViewMode(QListView::IconMode);
    m_pagesWidget->mListWidget->setFixedWidth( 200 );
}
void ConfigDialog::addWidget ( ConfigDialogPage * widget ) {
    m_pagesWidget->addWidget( widget, widget->title(), widget->subTitle(), widget->icon() );
}

void ConfigDialog::dialogAccepted(){
    bool b = true;
    QWidget * w;
    foreach ( w , m_pagesWidget->allWidgets() ){
        bool b = qobject_cast< ConfigDialogPage * >( w )->dialogAccepted();
        if (!b) break;
    }
    if (b)
        accept();
    else {
        //focus over first with error
        w->setFocus();
    }
}
ConfigDialog::~ConfigDialog()
{
}
