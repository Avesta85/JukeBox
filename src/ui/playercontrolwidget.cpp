#include "playercontrolwidget.h"
#include "ui_playercontrolwidget.h"

PlayerControlWidget::PlayerControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerControlWidget)
    ,m_isplaying(false)
    ,m_isMuted(false)
{
    ui->setupUi(this);
    ui->horizontalSlider_volume->setValue(100);

    connect(ui->pushButton_play, &QPushButton::clicked, this, &PlayerControlWidget::on_pushButton_play_clicked);
    connect(ui->pushButton_next, &QPushButton::clicked, this, &PlayerControlWidget::on_pushButton_next_clicked);
    connect(ui->pushButton_previous, &QPushButton::clicked, this, &PlayerControlWidget::on_pushButton_previous_clicked);
    connect(ui->pushButton_mute, &QPushButton::clicked, this, &PlayerControlWidget::on_pushButton_mute_clicked);
    connect(ui->pushButton_playing_type, &QPushButton::clicked, this, &PlayerControlWidget::on_pushButton_playing_type_clicked);
    connect(ui->horizontalSlider_volume, &QSlider::valueChanged, this, &PlayerControlWidget::on_horizontalSlider_volume_valueChanged);
    connect(ui->horizontalSlider_timeline, &QSlider::sliderMoved, this, &PlayerControlWidget::on_horizontalSlider_timeline_sliderMoved);
}

PlayerControlWidget::~PlayerControlWidget()
{
    delete ui;
}

void PlayerControlWidget::updatePlaybackState(bool isPlaying)
{
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


    // time lable update
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


void PlayerControlWidget::on_pushButton_play_clicked()
{
    if (m_isplaying) {
        emit pauseClicked();
    } else {
        emit playClicked();
    }
}

void PlayerControlWidget::on_pushButton_next_clicked()
{
    emit nextClicked();
}

void PlayerControlWidget::on_pushButton_previous_clicked()
{
    emit previousClicked();
}

void PlayerControlWidget::on_pushButton_playing_type_clicked()
{

    emit repeatModeClicked();
}

void PlayerControlWidget::on_pushButton_mute_clicked()
{

    emit muteClicked(!m_isMuted);
}

void PlayerControlWidget::on_horizontalSlider_volume_valueChanged(int value)
{
    emit volumeChanged(value);
}

void PlayerControlWidget::on_horizontalSlider_timeline_sliderMoved(int position)
{
    emit seeked(position);
}
