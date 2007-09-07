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
#include "styledialog.h"
#include "style.h"

#include <QList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QDir>
#include <QFileInfo>
#include <QScrollArea>
#include <QUrl>
#include <QDesktopServices>
#include <QPushButton>
#include <QMap>
#include <QApplication>
#include <QStyleFactory>


#ifdef Q_WS_WIN
    #define DATADIR QDir::homePath()+QDir::separator()+"zmviewer"
#else
    #define DATADIR QDir::homePath()+QDir::separator()+".zmviewer"
#endif

#define STYLESDIR DATADIR+QDir::separator()+"styles"

EntryButton::EntryButton( const Style &s , const QString &text,QWidget * parent )
:QPushButton( text, parent ){
      style  = new Style (s);
}
EntryView::EntryView( QWidget * parent )
:QLabel ( parent )
{
    

    connect(this, SIGNAL(linkActivated(const QString&)), SLOT(urlSelected(const QString&)));

  
}

void EntryView::setStyle( const Style &s ){
    setEntryViewStyle();
    QString authorString = s.author();
    QString t;
    QString emailString = s.authorEmail();
    if (!emailString.isEmpty())
    {
        authorString = "<a href='mailto:" + emailString + "'>"
            + authorString + "</a>";
    }
    t += QLatin1String("<table class='contentsHeader' cellspacing='2' cellpadding='0'>")
         + "<tr>"
         +   "<td><h2>" + s.name() + "</h2></td>"
         //+   "<td align='right'>" + starsString +  "</td>"
         +"<td align='right'></td>"
         + "</tr>"
         + "<tr>"
         +   "<td colspan='2' class='contentsBody'>"
         +    "Style: " + s.style()
         +   "</td>"
         + "</tr>"
         + "<tr>"
         +   "<td colspan='2' class='contentsFooter'>"
         +     "<em> by " + authorString + "</em> "
         +   "</td>"
         + "</tr>"
         + "</table>";
    t += "</qt>";
    setText( t );
}
void EntryView::setEntryViewStyle(){
    QString s;
    s += "body { background-color: white; color: black; padding: 0; margin: 0; }";
    s += "table, td, th { padding: 0; margin: 0; text-align: left; border:1px solid;}";
    s += "input { color: #000080; font-size:120%; }";

    s += ".contentsColumn { vertical-align: top; }";
    s += ".contentsHeader { width: 100%; font-size: 120%; font-weight: bold; border-bottom: 1px solid #c8c8c8; }";
    s += ".contentsBody {}";
    s += ".contentsFooter {}";
    setStyleSheet(s);

}

void EntryView::urlSelected( const QString & _url ){
    QUrl url ( _url );
    QDesktopServices::openUrl( url );
}

QPalette EntryView::defaultPalette(){
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    QPalette palette;
    palette.setBrush(QPalette::Active, QPalette::Window, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
    return palette;
}

class StyleDialog::Private{
  public:
  QList < Style > styles;
  QMap <Style , QPushButton *> map;
};

StyleDialog::StyleDialog( QWidget * parent )
 : QDialog( parent), d( new Private )
{
    init();
}

void StyleDialog::init(){

QDir dir ( DATADIR );
    if ( !dir.exists() ){
        if ( !dir.mkdir( DATADIR ) ){
                qWarning("StyleDialog::init(): error creating data directory. Styles disabled!");
                return;
            }
    }
dir.setPath( STYLESDIR );
    if ( !dir.exists() ){
        if ( !dir.mkdir( STYLESDIR ) ){
                qWarning("StyleDialog::init(): error creating style directory. Styles disabled!");
                return;
            }
    }
    foreach ( QFileInfo info , dir.entryInfoList() ){
        if ( info.fileName() == "." || info.fileName()=="..") continue;
        qDebug ( qPrintable(info.fileName() ) );
        if ( info.isDir () ){
                Style style ( info.absolutePath()+ QDir::separator()+  info.fileName() +  QDir::separator() + info.fileName() + ".xml");
                d->styles.append ( style );
        }
    }
    QVBoxLayout * _layout = new QVBoxLayout ( this );
    QLabel * labelTitle = new QLabel( tr("<h2>Select Styles:</h2>"), this);
    layout()->addWidget( labelTitle );
    QLabel * labelSubTitle = new QLabel( tr("You can Download more styles at <a href=http://zmviewer.sourceforge.net>http://zmviewer.sourceforge.net</a>"), this);
    connect  ( labelSubTitle, SIGNAL(linkActivated(const QString& ) ), this, SLOT(urlSelected(const QString&)) );
    layout()->addWidget( labelSubTitle );
    QScrollArea * scrollArea = new QScrollArea ( this );
    QVBoxLayout * scrollLayout = new QVBoxLayout( scrollArea );
    QWidget * container = new QWidget ( scrollArea );
    scrollLayout->addWidget  ( container );
    scrollLayout->addItem  ( new QSpacerItem( 20 , 40 , QSizePolicy::Minimum , QSizePolicy::Expanding ) );
    scrollArea->setWidgetResizable( true );
    scrollArea->setPalette( EntryView::defaultPalette() );
    scrollArea->setAutoFillBackground( true );
    layout()->addWidget( scrollArea );
    QGridLayout * widgetLayout = new QGridLayout ( container );
    int row = 0;
    QSize size;
    widgetLayout->setVerticalSpacing (10);
    foreach ( Style style , d->styles ){
        EntryView * view = new EntryView ( container );
        size+=view->sizeHint();
        view->setStyle ( style );
        widgetLayout->addWidget( view , row , 0 );
        EntryButton * setButton = new EntryButton( style , tr("Apply"), container );
        connect ( setButton , SIGNAL (clicked() ), this , SLOT(applyStyle() ) );
        widgetLayout->addWidget( setButton , row , 1 );
        //widgetLayout->addItem( new QSpacerItem( 20 , 20 , QSizePolicy::Expanding, QSizePolicy::Expanding  ) , row + 1 , 0);
        //widgetLayout->addLayout ( buttonLay );
        row++;
    }
    widgetLayout->addItem( new QSpacerItem( 20 , 40 , QSizePolicy::Minimum , QSizePolicy::Expanding ) , row , 0);
    scrollArea->setWidget ( container );
    QHBoxLayout * buttonBoxLayout = new QHBoxLayout ();
    QPushButton * closeButton = new QPushButton (tr( "Close") , this  );
    buttonBoxLayout->addItem( new QSpacerItem( 20 , 20 , QSizePolicy::Expanding, QSizePolicy::Minimum  ) );
    buttonBoxLayout->addWidget ( closeButton );
    connect ( closeButton , SIGNAL ( clicked()) , this, SLOT( close() ) );
    _layout->addLayout(buttonBoxLayout);
    resize( sizeHint() + container->sizeHint() + size );
   

}

void StyleDialog::urlSelected(const QString& _url ){
    QUrl url ( _url );
    QDesktopServices::openUrl( url );
  
}
void StyleDialog::applyStyle(){
  EntryButton * _sender = (EntryButton *)sender();
  qApp->setStyle( QStyleFactory::create( _sender->style->style() ) );
  qApp->setStyleSheet(  _sender->style->styleSheet() );
}
StyleDialog::~StyleDialog()
{
}



#include "styledialog.moc"
