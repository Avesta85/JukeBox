#include <QWidget>
#include <QPushButton>
#include "playercontrolwidget.h"
#include "ui_playercontrolwidget.h"
#include <QDebug>
#include <QIcon>

PlayerControlWidget::PlayerControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerControlWidget)
    ,m_isplaying(false)
    ,m_isMuted(false)
{
    ui->setupUi(this);
    ui->horizontalSlider_volume->setValue(100);

    connect(ui->pushButton_play, &QPushButton::clicked, this, &PlayerControlWidget::handlePlayClicked);
    connect(ui->pushButton_next, &QPushButton::clicked, this, &PlayerControlWidget::on_pushButton_next_clicked);
    connect(ui->pushButton_previous, &QPushButton::clicked, this, &PlayerControlWidget::on_pushButton_previous_clicked);
    connect(ui->pushButton_mute, &QPushButton::clicked, this, &PlayerControlWidget::handleMuteClicked);
    connect(ui->horizontalSlider_volume, &QSlider::valueChanged, this, &PlayerControlWidget::on_horizontalSlider_volume_valueChanged);
    connect(ui->horizontalSlider_timeline, &QSlider::sliderMoved, this, &PlayerControlWidget::on_horizontalSlider_timeline_sliderMoved);
    connect(ui->pushButton, &QPushButton::clicked, this, &PlayerControlWidget::onLikeButtonClicked);
    updateLikeIcon();
   // ui->pushButton_playing_type->setIcon(QIcon(":/icon.arrow.png"));
}

PlayerControlWidget::~PlayerControlWidget()
{
    delete ui;
}

void PlayerControlWidget::updatePlaybackState(bool isPlaying)
{
     m_isplaying = isPlaying;
    if(m_isplaying)
    {
        ui->pushButton_play->setIcon(QIcon(":/icone/pause.png"));

    }
    else
    {
        ui->pushButton_play->setIcon(QIcon(":/icone/play.png"));
    }
}


void PlayerControlWidget::updateVolume(int volume)
{
    ui->horizontalSlider_volume->blockSignals(true);
    ui->horizontalSlider_volume->setValue(volume);
    ui->horizontalSlider_volume->blockSignals(false);
}

void PlayerControlWidget::updatePosition(qint64 position)
{
    ui->horizontalSlider_timeline->blockSignals(true);
    ui->horizontalSlider_timeline->setValue(position);
    ui->horizontalSlider_timeline->blockSignals(false);


    int seconds = (position / 1000) % 60;
    int minutes = (position / 60000) % 60;
    ui->label_passed_time->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
}

void PlayerControlWidget::updateDuration(qint64 duration)
{
    ui->horizontalSlider_timeline->setRange(0, duration);

    int seconds = (duration / 1000) % 60;
    int minutes = (duration / 60000) % 60;
    ui->label_remained_time->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));

}

void PlayerControlWidget::setMuted(bool isMuted)
{

    m_isMuted = isMuted;

    if(isMuted) {
        ui->pushButton_mute->setIcon(QIcon(":/icone/mute.png"));
    } else {
        ui->pushButton_mute->setIcon(QIcon(":/icone/volume.png"));
    }
}


void PlayerControlWidget::handlePlayClicked()
{
    qDebug() << "[UI] handlePlayClicked() called";
    emit playPauseClicked();
    qDebug() << "[UI] playPauseClicked() emitted";
}

void PlayerControlWidget::on_pushButton_next_clicked()
{
    qDebug() << "[UI] on_pushButton_next_clicked() called";
    emit nextClicked();
    qDebug() << "[UI] nextClicked() emitted";
}

void PlayerControlWidget::on_pushButton_previous_clicked()
{
    qDebug() << "[UI] on_pushButton_previous_clicked() called";
    emit previousClicked();
    qDebug() << "[UI] previousClicked() emitted";
}

void PlayerControlWidget::on_pushButton_playing_type_clicked()
{
    qDebug() << "[UI] on_pushButton_playing_type_clicked() called";
    emit repeatModeClicked();
    qDebug() << "[UI] repeatModeClicked() emitted";
}

void PlayerControlWidget::handleMuteClicked()
{
    qDebug() << "[UI] handleMuteClicked() called";
    emit muteClicked(!m_isMuted);
    qDebug() << "[UI] muteClicked() emitted";
}

void PlayerControlWidget::on_horizontalSlider_volume_valueChanged(int value)
{
    qDebug() << "[UI] on_horizontalSlider_volume_valueChanged() called";
    emit volumeChanged(value);
    qDebug() << "[UI] volumeChanged() emitted";
}

void PlayerControlWidget::on_horizontalSlider_timeline_sliderMoved(int position)
{
    qDebug() << "[UI] on_horizontalSlider_timeline_sliderMoved() called";
    emit seeked(position);
    qDebug() << "[UI] seeked() emitted";
}

void PlayerControlWidget::setRepeatIcon(const QIcon& icon) {
    ui->pushButton_playing_type->setIcon(icon);
}

void PlayerControlWidget::setCurrentSong(qint64 songId, bool isFavorite)
{
    m_currentSongId = songId;
    m_isFavorite = isFavorite;
    updateLikeIcon();
}

void PlayerControlWidget::onLikeButtonClicked()
{
    if (m_currentSongId < 0) return;
    m_isFavorite = !m_isFavorite;
    updateLikeIcon();
    if (m_isFavorite)
        emit addFavorit(m_currentSongId);
    else
        emit deleteFavorite(m_currentSongId);
}

void PlayerControlWidget::updateLikeIcon()
{
    if (m_isFavorite)
        ui->pushButton->setIcon(QIcon(":/icone/like.png"));
    else
        ui->pushButton->setIcon(QIcon(":/icone/heart.png"));
}


