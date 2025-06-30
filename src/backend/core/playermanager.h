#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H
#include "src/backend/core/song.h"
#include <QObject>
#include <QMediaPlayer>
#include <QList>
#include <QUrl>
#include <src/backend/core/media.h>

class QAudioOutput;
class QUrl;
class QWidget;

class PlayerManager : public QObject
{
    Q_OBJECT

public:

    enum class RepeatMode{ NoRepeat,RepeatOne,RepeatAll};
    Q_ENUM(RepeatMode);
    // Singleton
    ~PlayerManager();
    static PlayerManager& getInstance();
    PlayerManager(const PlayerManager&) = delete;
    void operator=(const PlayerManager&) = delete;

public slots:

    void playSingleMedia(const QString & filepath);

    void loadSongPlaylist(const QList<Song*>& songPlaylist);
    void playFromPlaylist(int index);
    void next();
    void previous();

    void play();
    void pause();
    void stop();


    void seek(qint64 position);
    void setVolume(float volume);
    void setMuted(bool muted);

    void setShuffle(bool shuffle);
    void setRepeatMode(RepeatMode mode);

    void loadPlaylist(const QList<Song>& Songlist);
    void setVideoOutput(QWidget* videoWidget);
    void clearPlaylist();

    void addSong(const QString& filePath);

signals:


    void playlistChanged(const QList<Song>& playlist);
    void currentMediaChanged(Media* media);

    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void volumeChanged(float volume);
    void mutedChanged(bool muted);
    void currentSongChanged(const Song& song);
    void errorOccurred(QMediaPlayer::Error error, const QString& errorString);

private:
    explicit PlayerManager(QObject *parent = nullptr);
    static std::unique_ptr<PlayerManager> s_instance;

    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    // current media
    Media * currentMedia;

    //playlist

    QList<Song> m_playlist;

    QList<int>m_shuffledIndices;
    int m_currentIndex;
    bool m_isShuffled;
    RepeatMode m_repeatMode;



    void playIndex(int index);
    void cleanupCurrentMedia();

private slots:
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMetaDataChanged();
};

#endif // PLAYERMANAGER_H
