#include "stdio.h"
#include "vlcplayer.h"
#include <vlc/vlc.h>
#include <QTimer>

VlcPlayer::VlcPlayer(QObject * parent):
    AbstractPlayer(parent),
    m_vlcPlayer(NULL)
{
    /* Init libVLC "--network-caching=300"*/
    const char* arguments[2] = {"--no-video-title-show", "--clock-jitter=0"};
    if ( (m_vlcObject = libvlc_new(2, arguments)) == NULL ) {
        printf("Could not init libVLC");
        exit(1);
    }
}

VlcPlayer::~VlcPlayer()
{
    stop();
    if(m_vlcObject)
        libvlc_release(m_vlcObject);
}

void VlcPlayer::start(const QString &complete_url)
{
    m_complete_url = complete_url;
    QTimer::singleShot(100, this, SLOT(realStart()));
}

void VlcPlayer::stop()
{
    if(m_vlcPlayer) {
        libvlc_media_player_stop(m_vlcPlayer);
        //libvlc_media_player_set_position(m_vlcPlayer, 0.0);
        libvlc_media_player_release(m_vlcPlayer);
    }
    m_vlcPlayer = NULL;
}

void VlcPlayer::realStart()
{
    libvlc_media_t * m_vlcMedia = libvlc_media_new_location(m_vlcObject, m_complete_url.toStdString().c_str());
    if( !m_vlcMedia )
        return;

    m_vlcPlayer = libvlc_media_player_new_from_media(m_vlcMedia);
    libvlc_media_release(m_vlcMedia);

#if defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(m_vlcPlayer, (void *) m_frameWidget->winId());
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(m_vlcPlayer, m_frameWidget->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(m_vlcPlayer, m_frameWidget->winId());
#endif

    /* And play */
    libvlc_media_player_play(m_vlcPlayer);
}
