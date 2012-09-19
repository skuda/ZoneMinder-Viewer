#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>

#include "mjpegplayer.h"
#include "qmultipartreader.h"
#include "framewidget.h"

MjpegPlayer::MjpegPlayer(QObject * parent):
    AbstractPlayer(parent),
    m_currentPixmap(new QPixmap),
    m_multiPartReader(new QMultiPartReader("--ZoneMinderFrame", this)),
    m_manager(new QNetworkAccessManager(parent)),
    m_reply(NULL)
{
}

MjpegPlayer::~MjpegPlayer()
{
    if (m_reply)
        delete m_reply;
    delete m_manager;
    delete m_multiPartReader;
    delete m_currentPixmap;
}

void MjpegPlayer::start(const QString &complete_url)
{
    m_connStatus = Stream::Playing;
    m_finishedConnection = false;

    m_reply = m_manager->get(QNetworkRequest(QUrl(complete_url)));
    connect( m_reply, SIGNAL(readyRead()), this, SLOT(readImg()) );
    connect( m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(readError(QNetworkReply::NetworkError)) );
    connect( m_reply, SIGNAL(finished()), this, SLOT(finishedConnection()) );
}

void MjpegPlayer::stop()
{
    m_connStatus = Stream::Stopped;
    m_finishedConnection = true;

    if (m_reply) {
        disconnect( m_reply, SIGNAL(readyRead()), this, SLOT(readImg()) );
        disconnect( m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                    this, SLOT(readError(QNetworkReply::NetworkError)) );
        disconnect( m_reply, SIGNAL(finished()), this, SLOT(finishedConnection()) );
    }
}

void MjpegPlayer::readImg()
{
    if (m_reply->bytesAvailable()) {
        QByteArray array = m_multiPartReader->read( m_reply->readAll() );
        if ( !array.isNull() && m_currentPixmap->loadFromData(array)) {
            emit( frameReady(m_currentPixmap) );
        }
    }
}

void MjpegPlayer::readError ( QNetworkReply::NetworkError error ){
    if (error != QNetworkReply::NoError && m_connStatus != Stream::Stopped) {
        setStatus ( Stream::HTTPError );
        emit ( done( m_reply->errorString() ) );
    }
    /*else { //TODO
        if ( d->type == Stream::Event ){
            setStatus( Stream::Stopped );
            emit ( done( tr( "Event finished.") ) );
        } else {
            if ( m_connStatus != Stream::Stopped ){
                setStatus( Stream::NoSignal );
                emit ( done( tr( "Stopped by server. Press play to try again") ) );
            } else {
                setStatus( d->userStatus );
            }
        }
    }*/
}

/*void MjpegPlayer::setPixmap(const QPixmap *p)
{
    if (p->isNull())
        return;

    //QWidget * tmp_widget = m_frameWidget->parentWidget()->parentWidget();
    QObject * our_parent = m_frameWidget->parent(); //CameraWidget
    if (!our_parent) {
        qDebug("MjpegPlayer don't have a parent in setPixmap");
        return;
    }

    QWidget * tmp_widget = qobject_cast<QWidget*> (our_parent);
    if (!tmp_widget) {
        qDebug("cannot cast our_parent to QWidget in MjpegPlayer::setPixmap");
        return;
    }

    QWidget * window = tmp_widget->window();
    if ( tmp_widget->isVisible() && tmp_widget->isVisibleTo(window) ) {
        m_frameWidget->setPixmap(*p);
    }
    else {
        m_frameWidget->emitInvisibleUpdate();
    }
}
*/
