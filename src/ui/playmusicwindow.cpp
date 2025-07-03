#include "playmusicwindow.h"
#include "ui_playmusicwindow.h"
#include <QFileDialog>
#include <QTableWidgetItem>
playmusicwindow::playmusicwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::playmusicwindow)
{
    ui->setupUi(this);
    connect(ui->tableWidget_songs_of_playlist, &QTableWidget::cellDoubleClicked, this, &playmusicwindow::onSongDoubleClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &playmusicwindow::onSearchSongClicked);
}

playmusicwindow::~playmusicwindow()
{
    delete ui;
}

void playmusicwindow::updateSongList(const QList<Song> &songs)
{
    m_allSongsOriginal = songs;
    m_allSongs = songs;
    ui->tableWidget_songs_of_playlist->setRowCount(0);
    ui->tableWidget_songs_of_playlist->setColumnCount(1);
    ui->tableWidget_songs_of_playlist->setHorizontalHeaderLabels(QStringList() << "Song Name");
    for (int i = 0; i < songs.size(); ++i) {
        ui->tableWidget_songs_of_playlist->insertRow(i);
        QTableWidgetItem* item = new QTableWidgetItem(songs[i].getName());
        item->setData(Qt::UserRole, songs[i].getPath());
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_songs_of_playlist->setItem(i, 0, item);
    }
}

void playmusicwindow::onSongDoubleClicked(int row, int column)
{
    QTableWidgetItem* item = ui->tableWidget_songs_of_playlist->item(row, 0);
    if (!item) return;
    QString filePath = item->data(Qt::UserRole).toString();
    if (!filePath.isEmpty()) {
        emit songFileSelected(filePath);
    }
    this->hide();
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
    this->hide();
}

void playmusicwindow::onSearchSongClicked()
{
    QString searchText = ui->lineEdit_search_song->text().trimmed();
    QList<Song> filtered;
    if (searchText.isEmpty()) {
        filtered = m_allSongsOriginal;
    } else {
        for (const Song& s : m_allSongsOriginal) {
            if (s.getName().startsWith(searchText, Qt::CaseInsensitive)) {
                filtered.append(s);
            }
        }
    }
    ui->tableWidget_songs_of_playlist->setRowCount(0);
    for (int i = 0; i < filtered.size(); ++i) {
        ui->tableWidget_songs_of_playlist->insertRow(i);
        QTableWidgetItem* item = new QTableWidgetItem(filtered[i].getName());
        item->setData(Qt::UserRole, filtered[i].getPath());
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_songs_of_playlist->setItem(i, 0, item);
    }
}


