/**************************************************************************
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

#include "commandlistener.h"

#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QHash>
#include <QStringList>
#include <QRegExp>
#include <QAction>
#include <QTimer>

class CommandListener::Private{
public:
    QHash < QString , QAction *> actionHash;
    QStringList keyList;
    bool listening;
    QString buffer;
    QTimer *timer;
    int keyDelay;
};

CommandListener::CommandListener( QObject * parent )
 : QObject( parent ),d( new Private )
{
    init();
}

void CommandListener::init(){
    d->listening =  false;
    d->timer = new QTimer( this );
    d->keyDelay = 250;
    connect( d->timer, SIGNAL( timeout() ), this, SLOT( keyTimeout() ) );
}

void CommandListener::setMapAction( const QString & map, QAction * action ){
    d->actionHash.insert( map , action );
    d->keyList << map;
}

bool CommandListener::eventFilter(QObject *object, QEvent *event ){
    if ( event->type() == QEvent::KeyPress ) {
        d->timer->stop();
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        QString keyText = keyEvent->text();
        QStringList foundList ;
        if ( ! d->listening )
            foundList = d->keyList.filter(QRegExp ("^" + keyText + ".*$" ) );
        else foundList = d->keyList.filter(QRegExp ("^" + d->buffer + keyText + ".*$" ) );

        if ( foundList.count() > 0  ){
            if ( foundList.count() == 1 ){
                //trigger action
                d->actionHash[ foundList.first() ]->trigger();
                d->listening = false;
                d->buffer.clear();
            } else {
                //reserve key;
                d->buffer += keyText;
                d->listening = true;
                d->timer->start( d->keyDelay );
            }
        } else {
            d->buffer.clear();
            d->listening = false;
        }
        }
     return QObject::eventFilter( object, event );
}

void CommandListener::setKeyDelay( int delay ){
    d->keyDelay = delay;
}

int CommandListener::keyDelay( )const{
    return d->keyDelay;
}

void CommandListener::clear(){
    d->actionHash.clear();
}

void CommandListener::keyTimeout(){
    if ( d->actionHash.contains( d->buffer ) )
        d->actionHash[ d->buffer ]->trigger();

    d->buffer.clear();
    d->listening = false;
    d->timer->stop();
}

CommandListener::~CommandListener()
{
    delete d;
}

#include "commandlistener.moc"
