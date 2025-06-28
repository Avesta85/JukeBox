#ifndef EMAILVERIFICATIONWINDOW_H
#define EMAILVERIFICATIONWINDOW_H
#include <QString>
#include <QMessageBox>
#include <QDialog>
#include "changepasswordwindow.h"

namespace Ui {
class EmailVerificationWindow;
}

class EmailVerificationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmailVerificationWindow(QWidget *parent = nullptr);
    ~EmailVerificationWindow();
    void Emailsender(QString email);
    void setResiverUsername(QString username);

signals:
    void backToForgetPassWindow();
    void passVerification_gotoChangepassword(QString Username);


private slots:
    void on_pushButton_Back_clicked();

    void on_pushButton_coniform_clicked();

private:
    QString reciver_username;
    QString sended_email;
    Ui::EmailVerificationWindow *ui;
    ChangePasswordWindow* changePassWindow;
};

#endif // EMAILVERIFICATIONWINDOW_H
