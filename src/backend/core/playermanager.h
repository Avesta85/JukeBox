#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "qvideowidget.h"
#include "src/backend/core/song.h"
#include <QObject>
#include <QMediaPlayer>
#include <QList>
#include <QVector>

class QAudioOutput;

class PlayerManager : public QObject
{
    Q_OBJECT

public:
    enum class PlayMode { SingleMedia, Playlist };
    enum class RepeatMode { Shuffle , RepeatOne, RepeatAll };
    Q_ENUM(RepeatMode);

    static PlayerManager& getInstance();
    ~PlayerManager();
    PlayerManager::RepeatMode getRepeatMode();
public slots:

    void loadSingleMedia(const QString& filePath);
    void loadSingleVideo(const QString& filePath, QVideoWidget* videoWidget);
    void loadPlaylist(const QList<Song>& playlist);
    void play();
    void pause();
    void next();
    void previous();
    void changeRepeatMode();
    void setVolume(float volume);
    void setMuted(bool muted);
    void seek(qint64 position);
    void togglePlayPause();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

signals:

    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void currentSongChanged(const Song& song);
    void volumeChanged(int volume);
    void mutedChanged(bool isMuted);

private:
    explicit PlayerManager(QObject *parent = nullptr);

    static std::unique_ptr<PlayerManager> s_instance;

    void playSongAtIndex(int index);
    void cleanupCurrentMedia();
    void generateShuffleIndexes();

    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;

    Song* m_currentMedia;
    QList<Song> m_playlist;

    PlayMode m_playMode;
    RepeatMode m_repeatMode;
    int m_currentIndex;
    float m_volumeBeforeMute;

    QVector<int> m_shuffleIndexes;
    int m_currentShuffleIndex = 0;
};

#endif // PLAYERMANAGER_H
