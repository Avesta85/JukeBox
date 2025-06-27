#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QRegularExpression>

namespace Ui {
class SignupWindow;
}

class SignupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SignupWindow(QWidget *parent = nullptr);
    ~SignupWindow();

signals:
    void backToChoiseWindow();
    void signUpsuccessfuly();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_signup_clicked();

private:
    Ui::SignupWindow *ui;
};

#endif // SIGNUPWINDOW_H
