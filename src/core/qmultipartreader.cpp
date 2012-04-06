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
#include "qmultipartreader.h"


#include <QDebug>
#include <QByteArray>
#include <QBuffer>
#include <QHttpResponseHeader>
//#define DEBUG_PARSING
class QMultiPartReader::Private{
    public:
    bool m_bParsingHeader;
    bool m_bGotAnyHeader;
    bool m_gzip;
    QString m_mimeType;
    QString m_nextMimeType;
    QByteArray m_boundary;
    int m_boundaryLength;
    //test vars
    //bool m_haveLength;
    //bool m_haveMimeType;

    QBuffer buffer;
};

/**
    KLineParser from kde4 kdelibs/khtml/kmultipart.cpp and renamed to QLineParser
*/
class QLineParser
{
public:
    QLineParser() {
        m_lineComplete = false;
    }
    void addChar( char c, bool storeNewline ) {
        if ( !storeNewline && c == '\r' )
            return;
        Q_ASSERT( !m_lineComplete );
        if ( storeNewline || c != '\n' ) {
            int sz = m_currentLine.size();
            m_currentLine.resize( sz+1 );
            m_currentLine[sz] = c;
        }
        if ( c == '\n' )
            m_lineComplete = true;
    }
    bool isLineComplete() const {
        return m_lineComplete;
    }
    QByteArray currentLine() const {
        return m_currentLine;
    }
    void clearLine() {
        Q_ASSERT( m_lineComplete );
        reset();
    }
    void reset() {
        m_currentLine.resize( 0 );
        m_lineComplete = false;
    }
private:
    QByteArray m_currentLine;
    bool m_lineComplete; // true when ending with '\n'
};


QMultiPartReader::QMultiPartReader( const QByteArray &boundary, QObject * parent )
:QObject ( parent ), d( new Private )
{
    d->m_boundary = boundary;
    d->m_boundaryLength = d->m_boundary.length();
    startHeader();
    m_lineParser = new QLineParser;
    m_lineParser->reset();
    d->buffer.open(QIODevice::ReadWrite);

}

void QMultiPartReader::startHeader(){
    d->m_bParsingHeader = true; // we expect a header to come first
    d->m_bGotAnyHeader = false;
    d->m_gzip = false;
//   test code
//    m_haveLength = false;
//    m_haveMimeType = false;
}

QByteArray QMultiPartReader::read( const QByteArray  &data ){
    for ( int i = 0; i < data.size() ; ++i )
    {
        // Store char. Skip if '\n' and currently parsing a header.
        m_lineParser->addChar( data[i], !d->m_bParsingHeader );
        if ( m_lineParser->isLineComplete() )
        {
            QByteArray lineData = m_lineParser->currentLine();

            QByteArray line( lineData.data(), lineData.size()+1 ); // deep copy
            // 0-terminate the data, but only for the line-based tests below
            // We want to keep the raw data in case it ends up in sendData()
            int sz = line.size();
            if ( sz > 0 )
                line[sz-1] = '\0';

            if ( d->m_bParsingHeader )
            {
                if ( !line.isEmpty() )
                    d->m_bGotAnyHeader = true;
                 if ( !qstrnicmp( line.data(), "Content-Length:", 15 ) )
                {
                    QString length = QString::fromLatin1(line.data()+15).trimmed().toLower();
                #ifdef DEBUG_PARSING
                    qDebug ( ) << "Content-Length: " << length;
                #endif
    //test code                    m_haveLength = true;
                }
                // parse Content-Type
                else if ( !qstrnicmp( line.data(), "Content-Type:", 13 ) )
                {
                    //Q_ASSERT( d->m_nextMimeType.isNull() );
                    d->m_nextMimeType = QString::fromLatin1( line.data() + 14 ).trimmed();
                    int semicolon = d->m_nextMimeType.indexOf( ';' );
                    if ( semicolon != -1 )
                        d->m_nextMimeType = d->m_nextMimeType.left( semicolon );
                #ifdef DEBUG_PARSING
                    qDebug() << "d->m_nextMimeType=" << d->m_nextMimeType;
                #endif
//test code                    m_haveMimeType = true;
                }
                // Empty line, end of headers (if we had any header line before)
                else if ( ( line.isEmpty() || line.data() == QByteArray("\r\n" ) || line.toHex() == "00" ) && d->m_bGotAnyHeader  )
                {
                    d->m_bParsingHeader = false;
            #ifdef DEBUG_PARSING
                    qDebug() << "end of headers";
            #endif
                    startOfData();
                }/*  test code
                            else if ( m_haveLength && m_haveMimeType){
                            qDebug() << "have all headers!!!";
                             qDebug() << ">>"<<line.data() << "<<";
                            qDebug() << line.toHex();
                            if ( line.isEmpty() ) qDebug () << "empty";
                            if ( line == "\r\n" ) qDebug () << "\\r\\n";
                            if ( line == "\r" ) qDebug () << "\\r";
                            if ( line == "\n" ) qDebug () << "\\n";
                            if ( line == " " ) qDebug () << "empty 2";
                            m_haveMimeType = m_haveLength = false;
                            //qDebug() << line.data();
                       }*/
                // First header (when we know it from kio_http)
                else if ( line == d->m_boundary )
                    {}; // nothing to do
#ifdef DEBUG_PARSING
                else if ( !line.isEmpty() ) // this happens with e.g. Set-Cookie:
                    qDebug() << "Ignoring header " << line;
#endif
            } else {
                if ( !qstrncmp( line, d->m_boundary, d->m_boundaryLength ) )
                {
#ifdef DEBUG_PARSING
                    qDebug() << "boundary found!";
                    qDebug() << "after it is " << line.data() + d->m_boundaryLength;
#endif

                    // Was it the very last boundary ?
                    if ( !qstrncmp( line.data() + d->m_boundaryLength, "--", 2 ) )
                    {
#ifdef DEBUG_PARSING
                        qDebug() << "Completed!";
#endif
                        QByteArray ret = endOfData();
                        emit completed();
                        m_lineParser->clearLine();
                        return ret;
                    } else
                    {
                        char nextChar = *(line.data() + d->m_boundaryLength);
#ifdef DEBUG_PARSING
                        qDebug() << "QMultiPartReader::slotData nextChar='" << nextChar << "'";
#endif
                        if ( nextChar == '\n' || nextChar == '\r' ) {
                            QByteArray ret = endOfData();
                            m_lineParser->clearLine();
                            startHeader();
                            return ret;
                        }
                        else {
                            // otherwise, false hit, it has trailing stuff
                            d->buffer.write(m_lineParser->currentLine());
                        }
                    }
                } else {
                    d->buffer.write(m_lineParser->currentLine());
                }
            }
            m_lineParser->clearLine();
        } //else qDebug() << "Line not complete";
    }
    //d->c_frame->loadFromData( d->http->readAll() );
    //d->label->setPixmap( *d->c_frame );
    //qDebug ( qPrintable ( con ) );
    return QByteArray();
}

void QMultiPartReader::startOfData()
{
    #ifdef DEBUG_PARSING
    qDebug() << "QMultiPartReader::startOfData";
    #endif
    d->m_bParsingHeader = false;
    Q_ASSERT( !d->m_nextMimeType.isNull() );
    if( d->m_nextMimeType.isNull() )
        return;

    if ( d->m_gzip )
    {
        qWarning ("QMultiPartReader:: GZip not supported for now!");
    }

    
}

/**
    TODO
*/
QString QMultiPartReader::setBoundaryFromHeader( const QHttpResponseHeader & header ){
    QString tmp = header.value ("boundary");
    if (tmp.startsWith(QLatin1String("--")))
        d->m_boundary = tmp.toLatin1();
    else
        d->m_boundary = QByteArray("--")+tmp.toLatin1();
    d->m_boundaryLength = d->m_boundary.length();
    return d->m_boundary;
}

QByteArray QMultiPartReader::endOfData()
{
        if ( !d->buffer.data().isNull() ){
            //we have a Frame!!!
            QByteArray ret = d->buffer.data();
            emit ( frameReady( ret ) );
            d->buffer.reset();
            return ret;
        }
    else return QByteArray();
}

QMultiPartReader::~QMultiPartReader()
{
    delete d;
    delete m_lineParser;
}

#include "qmultipartreader.moc"


