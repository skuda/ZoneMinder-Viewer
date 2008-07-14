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
#include "languageconfigwidget.h"
#include "translator.h"

#include <QDir>
#include <QTranslator>

LanguageConfigWidget::LanguageConfigWidget( QWidget * parent )
:ConfigDialogPage( parent )
{
    setupUi( this );
    setIcon( QIcon(":/config/Language") );
    setTitle( tr("Language") );
    setSubTitle( tr("Select your Language") );
    m_translator = Translator::instance();
    connect( m_languageComboBox, SIGNAL( currentIndexChanged ( int ) ), this, SLOT( setLocaleFromComboBox( int ) ) );
    initLanguageComboBox();
}

void LanguageConfigWidget::initLanguageComboBox()
{
    QDir dir(":/translations");

    m_languageComboBox->addItem( "English", "en" );

    foreach (QString fileName, dir.entryList() )
    {
        QString locale = fileName;
        locale.remove(0, locale.indexOf('_') + 1);
        locale.truncate(locale.lastIndexOf('.'));
        QTranslator translator;
        translator.load(fileName, ":/translations");
        QString language = translator.translate("Translate",
                                                "English");
        if ( language.isEmpty() ) language = locale;
        m_languageComboBox->addItem( language, locale );
    }
    //try search complete locale name
    int currentIndex =  m_languageComboBox->findData( m_translator->currentLocale() );
    if ( currentIndex < 0 ){
        //if complete not found try with only two chars
        currentIndex = m_languageComboBox->findData( m_translator->currentLocale().left( 2 ) );
    }
    m_languageComboBox->setCurrentIndex( currentIndex );

}

void LanguageConfigWidget::setLocaleFromComboBox( int index ){
    m_localeString = m_languageComboBox->itemData( index ).toString();
}

bool LanguageConfigWidget::dialogAccepted(){
    if ( !m_localeString.isEmpty() ){
        int c = m_localeString.count();
        if ( m_localeString != m_translator->currentLocale().left( c ) ){
            m_translator->loadLanguage( m_localeString );
            m_translator->writeSettings();
        }
    }
    return true;
}



LanguageConfigWidget::~LanguageConfigWidget()
{
}

#include "languageconfigwidget.moc"
