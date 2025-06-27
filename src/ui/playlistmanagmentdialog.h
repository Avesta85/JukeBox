#ifndef PLAYLISTMANAGMENTDIALOG_H
#define PLAYLISTMANAGMENTDIALOG_H

#include <QDialog>

namespace Ui {
class PlayListManagmentDialog;
}

class PlayListManagmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayListManagmentDialog(QWidget *parent = nullptr);
    ~PlayListManagmentDialog();

private:
    Ui::PlayListManagmentDialog *ui;
};

#endif // PLAYLISTMANAGMENTDIALOG_H
