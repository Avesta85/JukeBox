#ifndef DIALOG_FAVORITESONGS_H
#define DIALOG_FAVORITESONGS_H

#include "src/backend/core/song.h"
#include <QDialog>

namespace Ui {
class Dialog_FavoriteSongs;
}

class Dialog_FavoriteSongs : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_FavoriteSongs(QWidget *parent = nullptr);
    ~Dialog_FavoriteSongs();

public slots:
    void update_favoriteSongs(const QList<Song> FavoriteSongs);

signals:
    void playSongsRequest(const QString filepath);

private:
    Ui::Dialog_FavoriteSongs *ui;
    QList<Song> m_allSongs;
    void fill_table(const QList<Song>& list);
};

#endif // DIALOG_FAVORITESONGS_H
