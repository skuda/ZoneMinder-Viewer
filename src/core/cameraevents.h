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
#ifndef CAMERAEVENTS_H
#define CAMERAEVENTS_H

/**
	@author Leandro Emanuel Lopez <lopezlean@gmail.com>
*/
#include <QDialog>
class QModelIndex;
class QSqlTableModel;
class QTableView;
class CameraWidget;
class QCheckBox;
class QDate;
class QPushButton;
class QTimer;

class CameraEventCalendar;
class QTableWidgetItem;

class CameraEvents: public QDialog{
Q_OBJECT
public:
    enum DatabaseFields{
        Id,
        MonitorId,
        Name,
        Cause,
        StartTime,
        EndTime,
        Width,
        Height,
        Length,
        Frames,
        AlarmFrames,
        TotScore,
        AvgScore,
        MaxScore,
        Archivied,
        Videoed,
        Uploaded,   
        Emailed,
        Messaged,
        Executed,
        LearnState,
        Notes
    };
    CameraEvents( int camId, const QString & connectionName , QWidget * parent = 0 );
    ~CameraEvents();
    void appendZMSString( const QString & s );
    CameraWidget * cameraWidget( ) const;
public Q_SLOTS:
    void showOurEvent (const QModelIndex & index);
    void filterEventDate( const QDate & date );
    void restoreFilter( );
protected:
    void resizeEvent ( QResizeEvent * event );
protected Q_SLOTS:
    void updateDeleteButton( const QModelIndex & index );
    void deleteEvent();
    void updateEvents();
private:
    void init();
    int m_cameraId;
    QString m_connectionName;
    QSqlTableModel * m_model;
    QTableView * m_view;
    CameraWidget * m_camera;
    QString m_appendString;
    CameraEventCalendar * m_calendarWidget;
    QPushButton *m_clearFilterButton;
    QPushButton *m_deleteButton;
    QTimer * m_timer;
};

#endif
