#ifndef DIALOG_QUEUE_H
#define DIALOG_QUEUE_H

#include "src/backend/core/song.h"
#include <QDialog>

namespace Ui {
class Dialog_Queue;
}

class Dialog_Queue : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Queue(QWidget *parent = nullptr);
    ~Dialog_Queue();

public slots:
    void show_songs_list(const QList<Song> list);
signals:
    void play_Queue(const QList<qint64> songsID);
private slots:
    void on_pushButton_play_clicked();
    void on_pushButton_clicked();

private:
    Ui::Dialog_Queue *ui;
    QList<Song> m_allSongs;
    QList<Song> m_filteredSongs;
    void fill_table(const QList<Song>& list);
};

#endif // DIALOG_QUEUE_H

