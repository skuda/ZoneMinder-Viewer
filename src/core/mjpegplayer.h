#ifndef MJPEGPLAYER_H
#define MJPEGPLAYER_H

#include "abstractplayer.h"
#include <QObject>

class QMultiPartReader;
class QPixmap;
class QNetworkAccessManager;
class QNetWorkReply;
class FrameWidget;


class MjpegPlayer: public AbstractPlayer
{
    Q_OBJECT
public:
    MjpegPlayer(QObject * parent = 0);
    virtual ~MjpegPlayer();

    virtual void start(QString const &complete_url);
    virtual void stop();

Q_SIGNALS:
    void frameReady(QPixmap*);

private Q_SLOTS:
    void readImg();
    void readError(QNetworkReply::NetworkError);

private:
    QPixmap * m_currentPixmap;
    QMultiPartReader * m_multiPartReader;
    QNetworkAccessManager * m_manager;
    QNetworkReply * m_reply;
    FrameWidget * m_frameWidget;
};

#endif // MJPEGPLAYER_H
