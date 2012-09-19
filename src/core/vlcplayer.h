#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include "abstractplayer.h"
#include <QObject>
#include <QString>

class libvlc_instance_t;
class libvlc_media_player_t;
class FrameWidget;

class VlcPlayer: public AbstractPlayer
{
    Q_OBJECT
public:
    VlcPlayer(QObject * parent = 0);
    virtual ~VlcPlayer();

    virtual void start(QString const &complete_url);
    virtual void stop();

private Q_SLOTS:
    void realStart();

private:
    QString m_complete_url;
    libvlc_instance_t * m_vlcObject;
    libvlc_media_player_t * m_vlcPlayer;
};

#endif // VLCPLAYER_H
