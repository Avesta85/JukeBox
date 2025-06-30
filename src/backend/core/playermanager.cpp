#include "src/backend/core/playermanager.h"
#include "qeventloop.h"
#include "qfileinfo.h"
#include "qrandom.h"
#include <QAudioOutput>
#include <QWidget>
#include <QDebug>
#include <QMediaMetaData>

std::unique_ptr<PlayerManager> PlayerManager::s_instance = nullptr;

PlayerManager& PlayerManager::getInstance() {
    if (!s_instance) {
        s_instance.reset(new PlayerManager());
    }
    return *s_instance;
}

void PlayerManager::playSingleMedia(const QString &filepath)
{
    stop();
    clearPlaylist();
    cleanupCurrentMedia();

    m_currentIndex = -1;

    // TODO: اینجا باید تشخیص دهید فایل ورودی آهنگ است یا فیلم
    // فعلاً فرض می‌کنیم آهنگ است
    currentMedia = new Song();
    currentMedia->setPath(filepath);
    currentMedia->setName(QFileInfo(filepath).baseName());

    m_player->setSource(QUrl::fromLocalFile(filepath));
    play();
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
        m_player->pause();
        return;
    }


    if(m_player->playbackState() == QMediaPlayer::StoppedState)
    {
        if(m_currentIndex != -1 && !m_playlist.isEmpty())
        {
            playIndex(m_currentIndex);
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
    cleanupCurrentMedia();

    m_playlist = Songlist;
    emit playlistChanged(m_playlist);
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
    m_playlist.clear();
}

void PlayerManager::addSong(const QString &filePath)
{
    m_player->setSource(QUrl::fromLocalFile(filePath));

    current_song = Song();
    current_song.setPath(filePath);
    current_song.setName(QFileInfo(filePath).baseName());
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
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &PlayerManager::onMetaDataChanged);

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


    // if(currentMedia)
    // {
    //     delete currentMedia;
    // }
    const int playlistIndex = m_isShuffled ? m_shuffledIndices.at(index) : index;
    currentMedia = new Song(m_playlist.at(playlistIndex));
    if(currentMedia)
    {
        m_player->setSource(QUrl::fromLocalFile(currentMedia->getPath()));
        emit currentMediaChanged(currentMedia);
        play();
    }
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

void PlayerManager::onMetaDataChanged()
{
    qDebug() << "Meta-data is now available!";
    QString title = m_player->metaData().value(QMediaMetaData::Title).toString();
    if (!title.isEmpty()) {
        currentMedia->setName(title);
    }
    currentMedia->setDuration(m_player->duration());

    emit currentMediaChanged(currentMedia);
}

