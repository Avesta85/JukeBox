#ifndef FROGOTPASSWORDWINDOW_H
#define FROGOTPASSWORDWINDOW_H

#include <QDialog>

namespace Ui {
class FrogotPasswordWindow;
}

class FrogotPasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FrogotPasswordWindow(QWidget *parent = nullptr);
    ~FrogotPasswordWindow();

private:
    Ui::FrogotPasswordWindow *ui;
};

#endif // FROGOTPASSWORDWINDOW_H
