#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "frogotpasswordwindow.h"
#include "changepasswordwindow.h"
#include <QString>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    Ui::LoginWindow *ui;
    explicit LoginWindow(QWidget *parent = nullptr);

    ~LoginWindow();

signals:
    void backToMain();


private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_forgetpassword_clicked();

private:
    FrogotPasswordWindow* forgetPassWindow;
   ChangePasswordWindow* changePass;

};

#endif // LOGINWINDOW_H
