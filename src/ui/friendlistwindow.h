#ifndef FRIENDLISTWINDOW_H
#define FRIENDLISTWINDOW_H

#include <QDialog>

namespace Ui {
class FriendListWindow;
}

class FriendListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FriendListWindow(QWidget *parent = nullptr);
    ~FriendListWindow();

private:
    Ui::FriendListWindow *ui;
};

#endif // FRIENDLISTWINDOW_H
