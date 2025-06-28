#ifndef DELETEPLAYLIST_H
#define DELETEPLAYLIST_H

#include <QDialog>

namespace Ui {
class DeletePlayList;
}

class DeletePlayList : public QDialog
{
    Q_OBJECT

public:
    explicit DeletePlayList(QWidget *parent = nullptr);
    ~DeletePlayList();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DeletePlayList *ui;
};

#endif // DELETEPLAYLIST_H
