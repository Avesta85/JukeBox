#ifndef EMAILVERIFICATIONWINDOW_H
#define EMAILVERIFICATIONWINDOW_H

#include <QDialog>

namespace Ui {
class EmailVerificationWindow;
}

class EmailVerificationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmailVerificationWindow(QWidget *parent = nullptr);
    ~EmailVerificationWindow();

private:
    Ui::EmailVerificationWindow *ui;
};

#endif // EMAILVERIFICATIONWINDOW_H
