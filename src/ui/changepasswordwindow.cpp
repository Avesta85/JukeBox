#include "changepasswordwindow.h"
#include "src/backend/db/DBM.h"
#include "src/backend/security/SecurityManager.h"
#include "ui_changepasswordwindow.h"

ChangePasswordWindow::ChangePasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordWindow)
{
    ui->setupUi(this);
}

ChangePasswordWindow::~ChangePasswordWindow()
{
    delete ui;
}

void ChangePasswordWindow::set_username(QString Username)
{
    this->Username = Username;
}

void ChangePasswordWindow::on_pushButton_clicked()
{
    QString password = ui->lineEdit_password->text();
    QString confirmPassword = ui->lineEdit_confirmePassword->text();

    if(password.isEmpty() || confirmPassword.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "You must fill the field!");
        return;
    }
    else if(password != confirmPassword)
    {
        QMessageBox::warning(this, "Warning", "password must bu equal with confirm password!");
        return;
    }

    // databace
    SecurityManager sm;
    if(DBM::get_instance().updateUserPassword(Username,sm.Hash(password)))
    {
        QMessageBox::information(this, "pass", "password changed");
    }
    else{
        QMessageBox::warning(this, "Warning", "password changing failed");
    }
    ui->lineEdit_password->clear();
    ui->lineEdit_confirmePassword->clear();

    emit goToLoginWindow();
}

