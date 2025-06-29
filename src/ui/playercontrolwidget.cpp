#include "playercontrolwidget.h"
#include "ui_playercontrolwidget.h"

PlayerControlWidget::PlayerControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerControlWidget)
{
    ui->setupUi(this);
    ui->horizontalSlider_volume->setValue(100);
}

PlayerControlWidget::~PlayerControlWidget()
{
    delete ui;
}

void PlayerControlWidget::on_pushButton_play_clicked()
{

    // mor condition before this
    if(playStatus == play)
    {
        ui->pushButton_play->setIcon(QIcon(":/icone/play"));
        playStatus = pause;
    }

    else if(playStatus == pause)
    {
        ui->pushButton_play->setIcon(QIcon(":/icone/pause"));
        playStatus = play;
    }
}


void PlayerControlWidget::on_pushButton_playing_type_clicked()
{
    if(playingTipe == repeatOne)
    {
        ui->pushButton_playing_type->setIcon(QIcon(":/icone/repeat"));
        playingTipe = repeatPlayList;
    }
    else if(playingTipe == repeatPlayList)
    {
        ui->pushButton_playing_type->setIcon(QIcon(":/icone/shuffle"));
        playingTipe = shuffle;
    }
    else if(playingTipe == shuffle)
    {
        ui->pushButton_playing_type->setIcon(QIcon(":/icone/repeatone"));
        playingTipe = repeatOne;
    }
}


void PlayerControlWidget::on_pushButton_mute_clicked()
{
    if(valumeStatus == volume)
    {
        ui->pushButton_mute->setIcon(QIcon(":/icone/mute"));
        ui->horizontalSlider_volume->setValue(0);
        valumeStatus = mute;
    }
    else if(valumeStatus == mute)
    {
        ui->pushButton_mute->setIcon(QIcon(":/icone/volume"));
         ui->horizontalSlider_volume->setValue(10);
        valumeStatus = volume;
    }
}


void PlayerControlWidget::on_horizontalSlider_volume_valueChanged(int value)
{
    if(value == 0)
    {
        ui->pushButton_mute->setIcon(QIcon(":/icone/mute"));
        valumeStatus = mute;
    }
    else if(valumeStatus == mute)
    {
        ui->pushButton_mute->setIcon(QIcon(":/icone/volume"));
        valumeStatus = volume;
    }
}

