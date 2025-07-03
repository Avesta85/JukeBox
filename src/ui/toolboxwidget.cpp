#include "toolboxwidget.h"
#include "ui_toolboxwidget.h"

ToolBoxWidget::ToolBoxWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolBoxWidget)
{
    ui->setupUi(this);

    connect(ui->pushButton_movie_managment, &QPushButton::clicked, this, [this]() {
        emit videoManagementClicked();
    });
    connect(ui->pushButton_playlist_management, &QPushButton::clicked, this, [this]() {
        emit playlistManagementClicked();
    });
}

ToolBoxWidget::~ToolBoxWidget()
{
    delete ui;
}

void ToolBoxWidget::on_pushButton_playlist_management_clicked()
{
    emit playlistManagementClicked();
}

void ToolBoxWidget::on_pushButton_movie_managment_clicked()
{
    emit movieManagementClicked();
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
}


void ToolBoxWidget::on_pushButton_favorite_clicked()
{
    emit FavoriteSongsClicked();
}


void ToolBoxWidget::on_pushButton_3_clicked()
{
    emit FriendsListClicked();
}

void ToolBoxWidget::on_pushButton_Queue_clicked()
{
    emit QueueClicked();
}

void ToolBoxWidget::on_pushButton_online_clicked()
{
    emit onlineManagmentClicked();
}



