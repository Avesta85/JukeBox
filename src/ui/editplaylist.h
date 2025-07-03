#ifndef EDITPLAYLIST_H
#define EDITPLAYLIST_H

#include "src/backend/core/song.h"
#include <QDialog>
#include <QSet>

namespace Ui {
class EditPlayList;
}

class EditPlayList : public QDialog
{
    Q_OBJECT

public:
    explicit EditPlayList(QWidget *parent = nullptr);
    ~EditPlayList();

public slots:
    void update_SongView(const QList<Song>& AllSong, const QList<Song>& playlistSong, qint64 playlistId, const QString& playlistName);

signals:
    void update_songToPlaylist(qint64 PlaylistId, const QList<qint64>& newSong, const QList<qint64>& rmSong);
    void update_playlistName(qint64 PlaylistId, const QString& newName);

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_back_clicked();

private:
    Ui::EditPlayList *ui;
    QSet<qint64> m_initialPlaylistIds;
    qint64 m_playlistId;
    QString m_initialPlaylistName;
};

#endif // EDITPLAYLIST_H

