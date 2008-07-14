/***************************************************************************
 *   Copyright (C) 2008 by Leandro Emanuel López   *
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
#ifndef FRAME_H
#define FRAME_H

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
#include <QObject>
class QHttpResponseHeader;
class QHttp;
class QLineParser;

class QMultiPartReader: public QObject{
  Q_OBJECT
public:
    QMultiPartReader( const QByteArray &boundary , QObject * parent = 0 );
    QString setBoundaryFromHeader( const QHttpResponseHeader & header );
    ~QMultiPartReader();

Q_SIGNALS:
    void frameReady( const QByteArray & data );
    void completed();
public Q_SLOTS:
    QByteArray read( const QByteArray  &data );

protected:
    void startHeader();
    void startOfData();
    QByteArray endOfData();
private Q_SLOTS:
private:
    class Private;
    Private *d;
    QLineParser * m_lineParser;
};

#endif
