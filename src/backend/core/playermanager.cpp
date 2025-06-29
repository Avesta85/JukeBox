#include "src/backend/core/playermanager.h"
#include "qrandom.h"
#include <QAudioOutput>
#include <QWidget>
#include <QDebug>

std::unique_ptr<PlayerManager> PlayerManager::s_instance = nullptr;

PlayerManager& PlayerManager::getInstance() {
    if (!s_instance) {
        s_instance.reset(new PlayerManager());
    }
    return *s_instance;
}

void PlayerManager::play()
{
    if(m_player->playbackState() == QMediaPlayer::PausedState)
    {
        m_player->play();
        return;
    }

    if(m_player->playbackState() == QMediaPlayer::PlayingState)
    {
        return;
    }


    if(m_player->playbackState() == QMediaPlayer::StoppedState)
    {
        if(m_currentIndex != -1 && !m_playlist.isEmpty())
        {
            //playAtIndex(m_currentindex);
        }
        else if(currentMedia != nullptr)
        {
            m_player->setSource(QUrl::fromLocalFile(currentMedia->getPath()));
            m_player->play();
        }
    }
}

void PlayerManager::pause()
{

    m_player->pause();
}

void PlayerManager::stop()
{

    m_player->stop();
}

void PlayerManager::next()
{
    if(m_playlist.isEmpty()) return;

    if(m_currentIndex >= m_playlist.size()-1)
    {
        if(m_repeatMode == RepeatMode::RepeatAll)
        {
            playIndex(0);
        }
        else{
            stop();
        }
    }else{
        playIndex(m_currentIndex+1);
    }
}

void PlayerManager::previous()
{
    if (m_playlist.isEmpty()) return;


    if (m_currentIndex <= 0) {
        if (m_repeatMode == RepeatMode::RepeatAll) {

            playIndex(m_playlist.size() - 1);
        } else {

            playIndex(0);
        }
    } else {
        playIndex(m_currentIndex - 1);
    }
}

void PlayerManager::seek(qint64 position)
{

    m_player->setPosition(position);
}

void PlayerManager::setVolume(float volume)
{
    if(volume >=0.0f && volume <=1.0f){
        m_audioOutput->setVolume(volume);
    }
}

void PlayerManager::setMuted(bool muted)
{

    m_audioOutput->setMuted(muted);
}

void PlayerManager::setShuffle(bool shuffle)
{
    m_isShuffled = shuffle;
    if(m_isShuffled && !m_playlist.isEmpty())
    {
        m_shuffledIndices.resize(m_playlist.size());
        std::iota(m_shuffledIndices.begin(),m_shuffledIndices.end(),0);

        std::shuffle(m_shuffledIndices.begin(),m_shuffledIndices.end(),*QRandomGenerator::global());
    }
    else{
        m_shuffledIndices.clear();
    }
}

void PlayerManager::setRepeatMode(RepeatMode mode)
{
    m_repeatMode = mode;
}

void PlayerManager::loadPlaylist(const QList<Song> &Songlist)
{
    stop();
    m_playlist.clear();

    for(const Song& song:Songlist)
    {
        m_playlist.append(song);
    }
    m_currentIndex = m_playlist.isEmpty() ? -1 : 0;

    if (m_isShuffled) {
        setShuffle(true);
    }

}

void PlayerManager::setVideoOutput(QWidget *videoWidget)
{
    m_player->setVideoOutput(videoWidget);
}

void PlayerManager::clearPlaylist()
{
    stop();
    m_playlist.clear();
    m_shuffledIndices.clear();
    m_currentIndex = -1;
}




PlayerManager::~PlayerManager() {}

PlayerManager::PlayerManager(QObject *parent)
    : QObject(parent),
    m_currentIndex(-1),
    m_isShuffled(false),
    m_repeatMode(RepeatMode::NoRepeat)
{

    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &PlayerManager::playbackStateChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayerManager::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayerManager::durationChanged);
    connect(m_player, &QMediaPlayer::errorOccurred, this, &PlayerManager::errorOccurred);
    connect(m_audioOutput, &QAudioOutput::volumeChanged, this, &PlayerManager::volumeChanged);
    connect(m_audioOutput, &QAudioOutput::mutedChanged, this, &PlayerManager::mutedChanged);


    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &PlayerManager::handleMediaStatusChanged);
}

void PlayerManager::playIndex(int index)
{
    if(index <0 || index >=m_playlist.size())
    {
        stop();
        return;
    }

    m_currentIndex = index;

    const int playlistIndex = m_isShuffled ? m_shuffledIndices.at(index) : index;
    const Song& currentSong = m_playlist.at(playlistIndex);

    m_player->setSource(QUrl::fromLocalFile(currentSong.getPath()));
    emit currentSongChanged(currentSong);

    m_player->play();
}

void PlayerManager::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    emit mediaStatusChanged(status);

    if(status == QMediaPlayer::EndOfMedia)
    {
        if(m_repeatMode == RepeatMode::RepeatOne)
        {
            m_player->setPosition(0);
            m_player->play();
        }
        else{
            next();
        }
    }
}

