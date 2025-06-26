#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H

#include <QDialog>
#include "loginwindow.h"
#include "signupwindow.h"

namespace Ui {
class ChoiceWindow;
}

class ChoiceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChoiceWindow(QWidget *parent = nullptr);
    ~ChoiceWindow();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_signup_clicked();

private:
    Ui::ChoiceWindow *ui;
    LoginWindow* loginWindow;
    SignupWindow* signupWindow;
};

#endif // CHOICEWINDOW_H
