#ifndef PLAYLISTCHOICEWINDOW_H
#define PLAYLISTCHOICEWINDOW_H

#include "src/backend/core/playlist.h"
#include <QDialog>
#include <QMenu>

namespace Ui {
class PlaylistChoiceWindow;
}

class PlaylistChoiceWindow : public QDialog {
    Q_OBJECT
public:
    explicit PlaylistChoiceWindow(QWidget* parent = nullptr);
    ~PlaylistChoiceWindow();

public slots:
    void updatePlaylistView(const QList<Playlist> playlists);
signals:
    void createPlaylistRequested();
    void editPlaylistRequested(qint64 playlistId);
    void deletePlaylistRequested(qint64 PlaylistID);
    void playPlaylistRequested(qint64 PlaylistID);

private slots:
    void showContextMenu(const QPoint& pos);
    void onSearchClicked();
    void onCreateClicked();
    void onSongsAction();
    void onDeleteAction();
    void onPlayAction();
    void filterPlaylists(const QString& text);

private:
    Ui::PlaylistChoiceWindow* ui;
    QMenu* contextMenu;
    QString contextPlaylistName;
    qint64 contextPlaylistID;
    QList<Playlist> m_allPlaylists;
};

#endif // PLAYLISTCHOICEWINDOW_H

