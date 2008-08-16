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
#include "databasesession.h"
#include "connectionmanager.h"

#include <QComboBox>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPixmap>
#include <QSqlDatabase>
#include <QIntValidator>

DatabaseSession::DatabaseSession( QWidget * parent )
:QDialog ( parent )
{
    init();
}

void DatabaseSession::init(){

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    QFrame * frame = new QFrame( this );
    QPalette palette;
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, brush);
    QBrush brush1(QColor(255, 255, 255, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
    QBrush brush2(QColor(255, 255, 255, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
    frame->setPalette(palette);
    frame->setAutoFillBackground(true);
    frame->setFrameShadow( QFrame::Sunken );
    frame->setFrameShape( QFrame::Box );

    QHBoxLayout * hboxLayout = new QHBoxLayout(frame);
    QLabel * label = new QLabel(frame);
    label->setText( tr("<b>New Database Connection</b>") );
    hboxLayout->addWidget(label);

    QLabel * img = new QLabel(frame);
    img->setLayoutDirection(Qt::RightToLeft);
    img->setPixmap(QPixmap(":/images/Connect"));
    hboxLayout->addWidget(img);

    vboxLayout->addWidget(frame);

    QSpacerItem * spacerItem = new QSpacerItem(323, 41, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    vboxLayout->addItem(spacerItem);

    QGridLayout * gridLayout = new QGridLayout();
    QLabel * hostLabel = new QLabel( this );
    hostLabel->setText(tr("&Server"));
    gridLayout->addWidget( hostLabel, 0, 0, 1, 1);

    m_hostLE = new QLineEdit( this );
    gridLayout->addWidget( m_hostLE, 0, 1, 1, 1);

    QLabel * userNameLabel = new QLabel( this );
    userNameLabel->setText(tr("&User"));

    gridLayout->addWidget( userNameLabel , 1, 0, 1, 1);

    m_userNameLE = new QLineEdit( this );
    
    gridLayout->addWidget( m_userNameLE , 1, 1, 1, 1);

    QLabel * passwordLabel = new QLabel( this );
    passwordLabel->setText(tr("&Password"));

    gridLayout->addWidget(passwordLabel, 2, 0, 1, 1);

    m_passwordLE = new QLineEdit( this );
    m_passwordLE->setEchoMode( QLineEdit::Password );

    gridLayout->addWidget( m_passwordLE , 2, 1, 1, 1);

    vboxLayout->addLayout(gridLayout);

    QGroupBox * advancedBox = new QGroupBox( this );
    advancedBox->setWindowTitle( tr("Advanced Settings" ));
    QVBoxLayout * vboxLayout1 = new QVBoxLayout(advancedBox);
    QGridLayout * gridLayout1 = new QGridLayout();
    
    QLabel * databaseLabel = new QLabel(advancedBox);
    databaseLabel->setText(tr("&Database"));
    gridLayout1->addWidget( databaseLabel, 0, 0, 1, 1);

    m_databaseNameLE = new QLineEdit(advancedBox);

    gridLayout1->addWidget(m_databaseNameLE, 0, 1, 1, 1);

    QLabel * driversLabel = new QLabel(advancedBox);
    driversLabel->setText( tr("D&river") );
    gridLayout1->addWidget( driversLabel, 1, 0, 1, 1);

    m_driversCB = new QComboBox(advancedBox);
    m_driversCB->addItems(QSqlDatabase::drivers());

    gridLayout1->addWidget( m_driversCB, 1, 1, 1, 1);

    QLabel * portLabel = new QLabel(advancedBox);
    portLabel->setText( tr("&Port") );
    gridLayout1->addWidget( portLabel , 2, 0, 1, 1);

    m_portLE = new QLineEdit(advancedBox);
    m_portLE->setValidator ( new QIntValidator( 0 , 65535 , this ));
    gridLayout1->addWidget( m_portLE , 2, 1, 1, 1);

    QLabel * wwwPortLabel = new QLabel(advancedBox);
    wwwPortLabel->setText  ( tr ("&Web Server Port") );
    gridLayout1->addWidget( wwwPortLabel , 3, 0, 1, 1);

    m_wwwPortLE = new QLineEdit( advancedBox );
    m_wwwPortLE->setValidator ( new QIntValidator( 0 , 65535 , this ) );
    gridLayout1->addWidget( m_wwwPortLE , 3, 1, 1, 1);

    vboxLayout1->addLayout(gridLayout1);

    vboxLayout->addWidget(advancedBox);

    QHBoxLayout * hboxLayout1 = new QHBoxLayout();
    QDialogButtonBox * buttonBox = new QDialogButtonBox( this );
    buttonBox->setStandardButtons(QDialogButtonBox::Ok |QDialogButtonBox::Cancel|QDialogButtonBox::NoButton);
    connect ( buttonBox, SIGNAL(accepted()), this , SLOT(tryConnect()));
    connect (buttonBox, SIGNAL(rejected()), this , SLOT(reject()));

    hboxLayout1->addWidget(buttonBox);

    QPushButton *pushButton = new QPushButton( this );
    pushButton->setText( tr("More.." ) );
    pushButton->setCheckable(true);
    connect (pushButton , SIGNAL ( clicked( bool ) ) , advancedBox , SLOT (setVisible( bool )));

    hboxLayout1->addWidget(pushButton);

    vboxLayout->addLayout(hboxLayout1);

    hostLabel->setBuddy(m_hostLE);
    databaseLabel->setBuddy(m_databaseNameLE);
    driversLabel->setBuddy(m_driversCB);
    portLabel->setBuddy(m_portLE);
    wwwPortLabel->setBuddy( m_wwwPortLE );
    userNameLabel->setBuddy(m_userNameLE);
    passwordLabel->setBuddy(m_passwordLE);
    advancedBox->setVisible( pushButton->isChecked() );
    layout()->setSizeConstraint(QLayout::SetFixedSize);


    /**
        DEFAULT:
        MYSQL is DEFAULT DRIVER
        3306 DEFAULT port
        zoneminder DEFAULT Database
    */
    m_hostLE->setText( "localhost" );
    m_driversCB->setCurrentIndex( m_driversCB->findText("QMYSQL") );
    m_portLE->setText("3306");
    m_wwwPortLE->setText("80");
    m_databaseNameLE->setText("zm");
}

void DatabaseSession::tryConnect(){
    ConnectionManager c;
    bool  b = c.addConnection( m_driversCB->currentText() , m_hostLE->text() , m_databaseNameLE->text(), m_userNameLE->text() , m_passwordLE->text(), m_portLE->text().toInt(), m_wwwPortLE->text().toInt() );
    if (!b)
        QMessageBox::critical( this , tr("Error"), tr("Database connection error: %1").arg( ConnectionManager::lastErrorString() ) );
    else {
        emit( needUpdate());
        done(QDialog::Accepted);
        }
}

DatabaseSession::~DatabaseSession()
{
}

#include "databasesession.moc"

