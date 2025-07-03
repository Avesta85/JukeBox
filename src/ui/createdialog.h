#ifndef MYCREATEDIALOG_H
#define MYCREATEDIALOG_H

#include "src/backend/core/song.h"
#include <QDialog>
#include <QDir>
#include <QListWidgetItem>

namespace Ui {
class mycreateDialog;
}

class mycreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit mycreateDialog(QWidget *parent = nullptr);
    ~mycreateDialog();

public slots:
    void update_songsList(const QList<Song> songsList);

private slots:
    void on_pushButton_creat_clicked();

signals:
    void CreatePlaylist(const QString name,QList<qint64> songsID);

private:
    Ui::mycreateDialog *ui;
};

#endif // MYCREATEDIALOG_H
