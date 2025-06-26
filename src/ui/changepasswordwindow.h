#ifndef CHANGEPASSWORDWINDOW_H
#define CHANGEPASSWORDWINDOW_H

#include <QDialog>

namespace Ui {
class ChangePasswordWindow;
}

class ChangePasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordWindow(QWidget *parent = nullptr);
    ~ChangePasswordWindow();

private:
    Ui::ChangePasswordWindow *ui;
};

#endif // CHANGEPASSWORDWINDOW_H
