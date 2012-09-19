#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include "stream.h"
#include "framewidget.h"
#include <QObject>

class FrameWidget;

class AbstractPlayer: public QObject
{
    Q_OBJECT

public:
    explicit AbstractPlayer(QObject * parent = 0);
    virtual ~AbstractPlayer() {;}

    inline Stream::Status status() { return m_status; }
    void setStatus(Stream::Status status);
    virtual void start(QString const &complete_url) = 0;
    virtual void stop() = 0;
    void restart();
    void setFrameWidget(FrameWidget *frameWidget);

public Q_SLOTS:
    void checkConnection();
    void finishedConnection();

Q_SIGNALS:
    void done( const QString & message );

protected:
    Stream::Status m_status;
    // used to check connection errors
    Stream::Status m_connStatus;
    int m_reconnectCount;
    bool m_finishedConnection;
    FrameWidget * m_frameWidget;
};

#endif // ABSTRACTPLAYER_H
