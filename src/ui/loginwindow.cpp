#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}


LoginWindow::~LoginWindow()
{
    delete ui;

}

void LoginWindow::on_pushButton_ok_clicked()
{
    QString userNameField = ui->lineEdit_username->text();
    QString passField = ui->lineEdit_password->text();
    QRegularExpression regex("^[a-zA-Z0-9]*$");

    if(userNameField.isEmpty() || passField.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "All fields must be filled.");
        return;
    }

    else if (!regex.match(userNameField).hasMatch() || !regex.match(passField).hasMatch())
    {
        QMessageBox::warning(this, "Warning", "Fields can only contain a-z and 0-9.");
        ui->lineEdit_username->clear();
        return;
    }

    else if(passField.length()<8)
    {
        QMessageBox::warning(this, "Warning", "password must be begger than 8 charecter");
        return;
    }

    // messege...

    // databace...


    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    // will delete....


}


void LoginWindow::on_pushButton_cancel_clicked()
{

    emit CancelOperation();
    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();

}


void LoginWindow::on_pushButton_forgetpassword_clicked()
{

    emit ForgotPassword();
    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();

}

