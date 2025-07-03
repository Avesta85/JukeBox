#include "src/backend/core/playermanager.h"
#include "qfileinfo.h"
#include <QAudioOutput>
//#include <QWidget>
#include <QDebug>
#include <QMediaMetaData>
#include <QVideoWidget>
#include <algorithm>
#include <random>
//#include <QVBoxLayout>

std::unique_ptr<PlayerManager> PlayerManager::s_instance = nullptr;



PlayerManager::PlayerManager(QObject *parent)
    : QObject(parent),
    m_player(new QMediaPlayer(this)),
    m_audioOutput(new QAudioOutput(this)),
    m_currentMedia(nullptr),
    m_playMode(PlayMode::SingleMedia),
    m_repeatMode(RepeatMode::RepeatOne),
    m_currentIndex(-1)
{
    m_player->setAudioOutput(m_audioOutput);
    // اتصال سیگنال‌های داخلی به سیگنال‌های عمومی
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &PlayerManager::playbackStateChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayerManager::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayerManager::durationChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &PlayerManager::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
        qDebug() << "PlayerManager Error:" << error << "-" << errorString;
    });
    m_volumeBeforeMute = m_audioOutput->volume();
}

PlayerManager::~PlayerManager() {
    cleanupCurrentMedia();
    m_player->play();
}

PlayerManager::RepeatMode PlayerManager::getRepeatMode()
{
    return this->m_repeatMode;
}

void PlayerManager::cleanupCurrentMedia() {
    if (m_currentMedia) {
        delete m_currentMedia;
        m_currentMedia = nullptr;
    }
}

// === پیاده‌سازی منطق جدید ===

void PlayerManager::loadSingleMedia(const QString& filePath) {

    cleanupCurrentMedia();
    m_playlist.clear();
    m_currentIndex = -1;
    m_playMode = PlayMode::SingleMedia;

    m_currentMedia = new Song();
    m_currentMedia->setPath(filePath);
    m_currentMedia->setName(QFileInfo(filePath).baseName());

    m_player->setSource(QUrl::fromLocalFile(filePath));
    emit currentSongChanged(*m_currentMedia);
    m_player->play();
}


void PlayerManager::generateShuffleIndexes() {
    m_shuffleIndexes.clear();
    int n = m_playlist.size();
    m_shuffleIndexes.reserve(n);
    for (int i = 0; i < n; ++i) m_shuffleIndexes.push_back(i);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_shuffleIndexes.begin(), m_shuffleIndexes.end(), g);
    m_currentShuffleIndex = 0;
}

void PlayerManager::loadPlaylist(const QList<Song>& playlist) {
    cleanupCurrentMedia();
    m_playlist = playlist;
    m_currentIndex = m_playlist.isEmpty() ? -1 : 0;
    m_playMode = PlayMode::Playlist;
    if (m_repeatMode == RepeatMode::Shuffle && !m_playlist.isEmpty()) {
        generateShuffleIndexes();
        playSongAtIndex(m_shuffleIndexes[m_currentShuffleIndex]);
    } else if (!m_playlist.isEmpty()) {
        playSongAtIndex(0);
    }
}

void PlayerManager::togglePlayPause()
{
    qDebug() << "[PlayerManager] togglePlayPause() called";
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        pause();
    } else {
        play();
    }
}

void PlayerManager::play() {
    qDebug() << "[PlayerManager] play() called";
    m_player->play();
  //  if (m_analyzer) m_analyzer->resume();
}

void PlayerManager::pause() {
    qDebug() << "[PlayerManager] pause() called";
    m_player->pause();
   // if (m_analyzer) m_analyzer->pause();
}

void PlayerManager::playSongAtIndex(int index) {
   // if (index < 0 || index >= m_playlist.size()) return;

    m_currentIndex = index;
    const Song& songToPlay = m_playlist.at(index);


    m_player->setSource(QUrl::fromLocalFile(songToPlay.getPath()));
    emit currentSongChanged(songToPlay);
    m_player->play();
}

void PlayerManager::next() {
    if (m_playMode != PlayMode::Playlist || m_playlist.isEmpty()) return;
    if (m_repeatMode == RepeatMode::Shuffle) {
        m_currentShuffleIndex++;
        if (m_currentShuffleIndex >= m_shuffleIndexes.size()) {
            generateShuffleIndexes();
        }
        playSongAtIndex(m_shuffleIndexes[m_currentShuffleIndex % m_shuffleIndexes.size()]);
    } else {
        m_currentIndex = (m_currentIndex + 1) % m_playlist.size();
        playSongAtIndex(m_currentIndex);
    }
}

void PlayerManager::previous() {
    if (m_playMode != PlayMode::Playlist || m_playlist.isEmpty()) return;
    if (m_repeatMode == RepeatMode::Shuffle) {
        m_currentShuffleIndex--;
        if (m_currentShuffleIndex < 0) {
            m_currentShuffleIndex = m_shuffleIndexes.size() - 1;
        }
        playSongAtIndex(m_shuffleIndexes[m_currentShuffleIndex]);
    } else {
        m_currentIndex = (m_currentIndex - 1 + m_playlist.size()) % m_playlist.size();
        playSongAtIndex(m_currentIndex);
    }
}

void PlayerManager::setMuted(bool muted) {
    qDebug() << "[PlayerManager] setMuted(" << muted << ") called";
    if (muted) {
        if (!m_audioOutput->isMuted()) {
            m_volumeBeforeMute = m_audioOutput->volume();
            setVolume(0);
        }
    } else {
        setVolume(m_volumeBeforeMute);
    }
    m_audioOutput->setMuted(muted);
    emit mutedChanged(muted);
    qDebug() << "[PlayerManager] mutedChanged(" << muted << ") emitted";
}

void PlayerManager::changeRepeatMode() {
    // Cycle: Shuffle -> RepeatOne -> RepeatAll -> Shuffle ...
    if (m_repeatMode == RepeatMode::Shuffle) {
        m_repeatMode = RepeatMode::RepeatOne;
        // TODO: به UI خبر بده که آیکون را به RepeatOne تغییر دهد
        m_currentShuffleIndex = 0; // Reset shuffle index
    } else if (m_repeatMode == RepeatMode::RepeatOne) {
        m_repeatMode = RepeatMode::RepeatAll;
        // TODO: به UI خبر بده که آیکون را به RepeatAll تغییر دهد
        m_currentShuffleIndex = 0; // Reset shuffle index
    } else if (m_repeatMode == RepeatMode::RepeatAll) {
        m_repeatMode = RepeatMode::Shuffle;
        // TODO: به UI خبر بده که آیکون را به Shuffle تغییر دهد
        if (!m_playlist.isEmpty()) {
            generateShuffleIndexes();
        }
    }
}
PlayerManager& PlayerManager::getInstance() {
    if (!s_instance) {
        s_instance.reset(new PlayerManager());
    }
    return *s_instance;
}

void PlayerManager::seek(qint64 position)
{
    qDebug() << "[PlayerManager] seek(" << position << ") called";
    m_player->setPosition(position);
}

void PlayerManager::setVolume(float volume)
{
    qDebug() << "[PlayerManager] setVolume(" << volume << ") called";
    if(volume >= 0.0f && volume <= 1.0f){
        m_audioOutput->setVolume(volume);
        emit volumeChanged(static_cast<int>(volume * 100));
        qDebug() << "[PlayerManager] volumeChanged(" << static_cast<int>(volume * 100) << ") emitted";
        if (volume == 0.0f && !m_audioOutput->isMuted()) {
            m_audioOutput->setMuted(true);
            emit mutedChanged(true);
            qDebug() << "[PlayerManager] mutedChanged(true) emitted (from setVolume, slider to 0)";
        } else if (volume > 0.0f && m_audioOutput->isMuted()) {
            m_audioOutput->setMuted(false);
            emit mutedChanged(false);
            qDebug() << "[PlayerManager] mutedChanged(false) emitted (from setVolume, slider up)";
        }
    }
}

void PlayerManager::loadSingleVideo(const QString& filePath, QVideoWidget* videoWidget) {
    if (!videoWidget) return;
    m_player->setVideoOutput(videoWidget);
    m_player->setSource(QUrl::fromLocalFile(filePath));
    // اطلاعات ویدیو را به UI بفرست
    Song temp;
    temp.setPath(filePath);
    temp.setName(QFileInfo(filePath).baseName());
    emit currentSongChanged(temp);
    m_player->play();
}

void PlayerManager::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        if (m_playMode == PlayMode::Playlist && !m_playlist.isEmpty()) {
            if (m_repeatMode == RepeatMode::RepeatOne) {
                playSongAtIndex(m_currentIndex);
            } else if (m_repeatMode == RepeatMode::RepeatAll) {
                next();
            } else if (m_repeatMode == RepeatMode::Shuffle) {
                m_currentShuffleIndex++;
                if (m_currentShuffleIndex >= m_shuffleIndexes.size()) {
                    generateShuffleIndexes();
                }
                playSongAtIndex(m_shuffleIndexes[m_currentShuffleIndex % m_shuffleIndexes.size()]);
            }
        }
    }
}




