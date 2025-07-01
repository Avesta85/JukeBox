#ifndef PLAYMUSICWINDOW_H
#define PLAYMUSICWINDOW_H

#include <QWidget>
#include <QDialog>
#include "src/backend/core/song.h"

QT_BEGIN_NAMESPACE
namespace Ui { class playmusicwindow; }
QT_END_NAMESPACE

class playmusicwindow : public QDialog
{
    Q_OBJECT

public:
    explicit playmusicwindow(QWidget *parent = nullptr);
    ~playmusicwindow();


signals:
    void songFileSelected(const QString& filePath);


public slots:
    void updateSongList(const QList<Song>& songs);
    void onSongDoubleClicked(int row, int column);

private slots:

    void on_pushButton_select_clicked();
    void onSearchSongClicked();

private:
    Ui::playmusicwindow *ui;
    QList<Song> m_allSongs;
    QList<Song> m_allSongsOriginal;
};

#endif // PLAYMUSICWINDOW_H
