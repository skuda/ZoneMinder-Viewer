#include "abstractplayer.h"
#include "stream.h"

AbstractPlayer::AbstractPlayer(QObject *parent):
    QObject(parent),
    m_frameWidget(NULL)
{
    m_status = Stream::None;
    m_connStatus = Stream::None;
    m_reconnectCount = 0;
    m_finishedConnection = false;
}

void AbstractPlayer::restart()
{
    stop();
    Stream *tmp_stream = qobject_cast<Stream*> (parent());
    if (tmp_stream) { //should ever work, i test just in case
        start(tmp_stream->completeUrl());
    } else {
        qDebug("qobject_cast of AbstractPlayer->parent() to Stream failed!");
    }
}

void AbstractPlayer::setFrameWidget(FrameWidget *frameWidget)
{
    m_frameWidget = frameWidget;
}

void AbstractPlayer::checkConnection()
{
    if ( m_connStatus == Stream::Playing && m_finishedConnection ) {
        //try to connect
        restart();
    } else if ( m_reconnectCount != 0 && !m_finishedConnection ) {
        m_reconnectCount = 0;
    }
}

void AbstractPlayer::finishedConnection()
{
    m_finishedConnection = true;
}

void AbstractPlayer::setStatus(Stream::Status status)
{
    m_status = status;
}
