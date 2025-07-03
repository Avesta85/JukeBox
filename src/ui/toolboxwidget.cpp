#include "toolboxwidget.h"
#include "ui_toolboxwidget.h"
#include "playmusicwindow.h"
#include "src/backend/core/playermanager.h"
#include "QDebug"

ToolBoxWidget::ToolBoxWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolBoxWidget)
{
    ui->setupUi(this);

    connect(ui->pushButton_movie_managment, &QPushButton::clicked, this, [this]() {
        emit videoManagementClicked();
        ui->pushButton_visualizer->hide();
        ui->pushButton_playlist_management->hide();
    });
    connect(ui->pushButton_playlist_management, &QPushButton::clicked, this, [this]() {
        emit playlistManagementClicked();
        ui->pushButton_visualizer->show();
    });
}

ToolBoxWidget::~ToolBoxWidget()
{
    delete ui;
}

void ToolBoxWidget::disableMovieMode()
{
    ui->pushButton_visualizer->setEnabled(false);
}

void ToolBoxWidget::on_pushButton_playlist_management_clicked()
{
    emit playlistManagementClicked();
}

void ToolBoxWidget::on_pushButton_movie_managment_clicked()
{
    emit movieManagementClicked();
    playerMode = movie;
}

void ToolBoxWidget::on_pushButton_setting_clicked()
{
    emit settingsClicked();
}

void ToolBoxWidget::on_pushButton_about_us_clicked()
{
    emit aboutUsClicked();
}

void ToolBoxWidget::on_pushButton_clicked()
{
    emit SongManagementClicked();
    //ui->pushButton_visualizer->setEnabled(true);
    playerMode = songC;
    ui->pushButton_visualizer->show();
    ui->pushButton_visualizer->setText("Cover Art");
    ui->pushButton_playlist_management->show();
}


void ToolBoxWidget::on_pushButton_online_clicked()
{
    emit onlineManagmentClicked();
}


void ToolBoxWidget::on_pushButton_visualizer_clicked()
{
    if(playerMode == songC)
    {
        playerMode = songV;
        ui->pushButton_visualizer->setText("Visualizer");
        emit visualizerClicked();
    }

    else if(playerMode == songV)
    {
        playerMode = songC;
        ui->pushButton_visualizer->setText("Cover Art");
        emit coverArtClicke();
    }
}


