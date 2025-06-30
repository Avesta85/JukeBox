#include "playmusicwindow.h"
#include "ui_playmusicwindow.h"
#include <QFileDialog>
#include <QTableWidgetItem>
playmusicwindow::playmusicwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::playmusicwindow)
{
    ui->setupUi(this);



    ui->tableWidget_playlist_names->setColumnCount(1);
    ui->tableWidget_playlist_names->setHorizontalHeaderLabels({"Title"});
    ui->tableWidget_playlist_names->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_playlist_names->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget_playlist_names, &QTableWidget::cellDoubleClicked, this, &playmusicwindow::onSongDoubleClicked);
}

playmusicwindow::~playmusicwindow()
{
    delete ui;
}

void playmusicwindow::updateSongList(const QList<Song> &songs)
{
    ;
}

void playmusicwindow::onSongDoubleClicked(int row, int column)
{
    ;
}

void playmusicwindow::on_pushButton_select_song_clicked()
{
    ;
}

void playmusicwindow::on_pushButton_select_clicked()
{
    qDebug() << "1. Add Song button was clicked!";
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        tr("Select a Song"),
        QDir::homePath(),
        tr("Audio Files (*.mp3 *.wav)")
        );


    if (!filePath.isEmpty()) {

        emit songFileSelected(filePath);
    }
}

