#include "dialog_favoritesongs.h"
#include "ui_dialog_favoritesongs.h"
#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QList>
#include <QPushButton>
#include <QLineEdit>
#include "src/backend/core/song.h"

Dialog_FavoriteSongs::Dialog_FavoriteSongs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_FavoriteSongs)
{
    ui->setupUi(this);
    connect(ui->tableWidget_songs_of_playlist, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        int row = item->row();
        QTableWidgetItem *pathItem = ui->tableWidget_songs_of_playlist->item(row, 1);
        if (pathItem) {
            emit playSongsRequest(pathItem->text());
            this->hide();
        }
    });
    // Connect search button
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        QString searchText = ui->lineEdit_search_song->text();
        QList<Song> filtered;
        if (searchText.trimmed().isEmpty()) {
            filtered = m_allSongs;
        } else {
            for (const Song& s : m_allSongs) {
                if (s.getName().contains(searchText, Qt::CaseInsensitive))
                    filtered.append(s);
            }
        }
        fill_table(filtered);
    });
}

Dialog_FavoriteSongs::~Dialog_FavoriteSongs()
{
    delete ui;
}

void Dialog_FavoriteSongs::update_favoriteSongs(const QList<Song> FavoriteSongs)
{
    m_allSongs = FavoriteSongs;
    fill_table(m_allSongs);
}

void Dialog_FavoriteSongs::fill_table(const QList<Song>& list)
{
    ui->tableWidget_songs_of_playlist->clear();
    ui->tableWidget_songs_of_playlist->setRowCount(list.size());
    ui->tableWidget_songs_of_playlist->setColumnCount(2);
    QStringList headers;
    headers << "Name" << "Path";
    ui->tableWidget_songs_of_playlist->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < list.size(); ++i) {
        const Song &song = list[i];
        QTableWidgetItem *nameItem = new QTableWidgetItem(song.getName());
        QTableWidgetItem *pathItem = new QTableWidgetItem(song.getPath());
        ui->tableWidget_songs_of_playlist->setItem(i, 0, nameItem);
        ui->tableWidget_songs_of_playlist->setItem(i, 1, pathItem);
    }
    ui->tableWidget_songs_of_playlist->resizeColumnsToContents();
}
