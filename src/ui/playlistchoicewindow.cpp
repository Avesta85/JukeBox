#include "playlistchoicewindow.h"
#include "ui_playlistchoicewindow.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>

PlaylistChoiceWindow::PlaylistChoiceWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PlaylistChoiceWindow)
{
    ui->setupUi(this);

    // Context menu
    contextMenu = new QMenu(this);
    contextMenu->addAction("Songs", this, SLOT(onSongsAction()));
    contextMenu->addAction("Delete", this, SLOT(onDeleteAction()));
    contextMenu->addAction("Play", this, SLOT(onPlayAction()));

    connect(ui->tableWidget_playlists, &QTableWidget::customContextMenuRequested, this, &PlaylistChoiceWindow::showContextMenu);
    connect(ui->pushButton_search, &QPushButton::clicked, this, &PlaylistChoiceWindow::onSearchClicked);
    connect(ui->pushButton_create, &QPushButton::clicked, this, &PlaylistChoiceWindow::onCreateClicked);
    connect(ui->lineEdit_search, &QLineEdit::returnPressed, this, &PlaylistChoiceWindow::onSearchClicked);
}

PlaylistChoiceWindow::~PlaylistChoiceWindow() {
    delete ui;
}

void PlaylistChoiceWindow::updatePlaylistView(const QList<Playlist>playlists)
{
    // Only update m_allPlaylists if this is the full list (not a filtered list)
    if (playlists.size() >= m_allPlaylists.size() || m_allPlaylists.isEmpty()) {
        m_allPlaylists = playlists;
    }
    ui->tableWidget_playlists->setRowCount(0);
    for (int i = 0; i < playlists.size(); ++i) {
        ui->tableWidget_playlists->insertRow(i);
        ui->tableWidget_playlists->setItem(i, 0, new QTableWidgetItem(playlists[i].getName()));
        ui->tableWidget_playlists->item(i, 0)->setData(Qt::UserRole, QVariant::fromValue<qulonglong>(playlists[i].getId()));
    }
}

void PlaylistChoiceWindow::showContextMenu(const QPoint& pos) {
    QModelIndex index = ui->tableWidget_playlists->indexAt(pos);
    if (!index.isValid()) return;
    contextPlaylistID = ui->tableWidget_playlists->item(index.row(), 0)->data(Qt::UserRole).toULongLong();
    contextPlaylistName = ui->tableWidget_playlists->item(index.row(), 0)->text();
    contextMenu->exec(ui->tableWidget_playlists->viewport()->mapToGlobal(pos));
}

void PlaylistChoiceWindow::onSearchClicked() {
    QString text = ui->lineEdit_search->text();
    filterPlaylists(text);
}

void PlaylistChoiceWindow::filterPlaylists(const QString& text) {
    QList<Playlist> filtered;
    for (const Playlist& pl : m_allPlaylists) {
        if (pl.getName().contains(text, Qt::CaseInsensitive)) {
            filtered.append(pl);
        }
    }
    updatePlaylistView(filtered);
}

void PlaylistChoiceWindow::onCreateClicked() {
    emit createPlaylistRequested();
}

void PlaylistChoiceWindow::onSongsAction() {
    emit editPlaylistRequested(contextPlaylistID);
}

void PlaylistChoiceWindow::onDeleteAction() {
    emit deletePlaylistRequested(contextPlaylistID);
}

void PlaylistChoiceWindow::onPlayAction() {
    emit playPlaylistRequested(contextPlaylistID);
}

