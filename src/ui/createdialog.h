#ifndef CREATEDIALOG_H
#define CREATEDIALOG_H

#include "src/backend/core/song.h"
#include <QDialog>
#include <QDir>
#include <QListWidgetItem>

namespace Ui {
class CreateDialog;
}

class CreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDialog(QWidget *parent = nullptr);
    ~CreateDialog();

public slots:
    void update_songsList(const QList<Song> songsList);

private slots:
    void on_pushButton_creat_clicked();


signals:
    void CreatePlaylist(const QString name,QList<qint64> songsID);

private:
    Ui::CreateDialog *ui;
};

#endif // CREATEDIALOG_H
