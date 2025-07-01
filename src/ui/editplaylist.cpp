#include "editplaylist.h"
#include "ui_editplaylist.h"
#include <QTableWidgetItem>
#include <QSet>

EditPlayList::EditPlayList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditPlayList)
    , m_playlistId(-1)
{
    ui->setupUi(this);
}

EditPlayList::~EditPlayList()
{
    delete ui;
}

static QList<qint64> songListToIdList(const QList<Song>& songs) {
    QList<qint64> ids;
    for (const Song& s : songs) ids << s.getID();
    return ids;
}

void EditPlayList::update_SongView(const QList<Song>& AllSong, const QList<Song>& playlistSong, qint64 playlistId, const QString& playlistName)
{
    ui->tableWidget_allSong->setColumnCount(1);
    ui->tableWidget_allSong->setHorizontalHeaderLabels(QStringList() << "Song Name");
    ui->tableWidget_allSong->setRowCount(0);
    auto tmp = songListToIdList(playlistSong);
    QSet<qint64> playlistIds(tmp.begin(), tmp.end());
    m_initialPlaylistIds = playlistIds;
    m_playlistId = playlistId;
    m_initialPlaylistName = playlistName;
    int row = 0;
    // 1. Add playlist songs (checked, in order)
    for (const Song& s : playlistSong) {
        ui->tableWidget_allSong->insertRow(row);
        QTableWidgetItem* item = new QTableWidgetItem(s.getName());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        item->setData(Qt::UserRole, QVariant::fromValue<qint64>(s.getID()));
        ui->tableWidget_allSong->setItem(row, 0, item);
        row++;
    }
    // 2. Add remaining songs (unchecked)
    QSet<qint64> alreadyAddedIds = playlistIds;
    for (const Song& s : AllSong) {
        if (alreadyAddedIds.contains(s.getID())) continue;
        ui->tableWidget_allSong->insertRow(row);
        QTableWidgetItem* item = new QTableWidgetItem(s.getName());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setData(Qt::UserRole, QVariant::fromValue<qint64>(s.getID()));
        ui->tableWidget_allSong->setItem(row, 0, item);
        row++;
    }
    // Set playlist name
    ui->lineEdit_playlistName->setText(playlistName);
}

void EditPlayList::on_pushButton_save_clicked()
{
    QSet<qint64> checkedIds;
    for (int i = 0; i < ui->tableWidget_allSong->rowCount(); ++i) {
        QTableWidgetItem* item = ui->tableWidget_allSong->item(i, 0);
        if (item && item->checkState() == Qt::Checked) {
            checkedIds.insert(item->data(Qt::UserRole).toLongLong());
        }
    }
    QList<qint64> newSongs, removedSongs;
    for (qint64 id : checkedIds) {
        if (!m_initialPlaylistIds.contains(id)) newSongs << id;
    }
    for (qint64 id : m_initialPlaylistIds) {
        if (!checkedIds.contains(id)) removedSongs << id;
    }
    emit update_songToPlaylist(m_playlistId, newSongs, removedSongs);
    // Check for playlist name change
    QString newName = ui->lineEdit_playlistName->text().trimmed();
    if (!newName.isEmpty() && newName != m_initialPlaylistName) {
        emit update_playlistName(m_playlistId, newName);
    }
    this->accept();
}

void EditPlayList::on_pushButton_back_clicked()
{
    this->reject();
}
