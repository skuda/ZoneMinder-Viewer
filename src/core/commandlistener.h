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

#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

#include <QObject>

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
class QAction;
/**
    This class map an action with a key.
    Is like a shortcut but it has the particularity of
    delay the keypress when 2 or more actions map start with the same key map
*/
class CommandListener : public QObject
{
    Q_OBJECT
public:
    CommandListener( QObject * parent = 0 );
    /**
        Add an action to map with @param map
        When the key passed in @param map is pressed the action @param action is triggered
    */
    void setMapAction( const QString & map, QAction * action );
    /**
        Set keypress delay to @param delay
        This is util for example when you has two actions that start whith the same key
        in the map.
        Example: 1 and 12 . When you press the key 1, CommandListener delay the keypress and wait @param delay msec for the 2 key, if you not press 2 key CommandListener will trigger the action mapped in 1.
    */
    void setKeyDelay( int delay );
    /**
        @return the keypress delay
    */
    int keyDelay( )const;
    /**
     remove all mapped actions
    */
    void clear();
    ~CommandListener();
protected:
    bool eventFilter(QObject *object, QEvent *event );
protected Q_SLOTS:
    void keyTimeout();
private:
    void init();
    class Private;
    Private *d;

};

#endif
