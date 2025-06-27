#include "signupwindow.h"
#include "aplicationmanager.h"
#include "ui_signupwindow.h"

SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
{
    ui->setupUi(this);
}

SignupWindow::~SignupWindow()
{
    delete ui;
}

void SignupWindow::on_pushButton_cancel_clicked()
{
    ui->lineEdit_confirm_pass->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_lastname->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();

    this->close();
    AplicationManager* am = AplicationManager::instance();
    am->showChoiseWindow();
}


void SignupWindow::on_pushButton_signup_clicked()
{
    QString userName = ui->lineEdit_username->text();
    QString name = ui->lineEdit_name->text();
    QString lastName = ui->lineEdit_lastname->text();
    QString Password = ui->lineEdit_password->text();
    QString email = ui->lineEdit_email->text();
    QString confirmPass = ui->lineEdit_confirm_pass->text();

    QRegularExpression regex("^[a-zA-Z0-9]*$");
    QRegularExpression regexName("^[a-zA-Z]*$");
    QRegularExpression regexEmail("^[a-z0-9]+@gmail\\.com$");
;

    if(userName.isEmpty() || name.isEmpty() || lastName.isEmpty() || Password.isEmpty() || email.isEmpty() || confirmPass.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "All fields must be filled.");
        return;
    }

    else if (!regex.match(userName).hasMatch() || !regex.match(Password).hasMatch() || !regex.match(confirmPass).hasMatch())
    {
        QMessageBox::warning(this, "Warning", "usern name and password Fields can only contain a-z and 0-9.");
        return;
    }
    else if(!regexEmail.match(email).hasMatch())
    {
        QMessageBox::warning(this, "Warning", "Invalid email!");
        return;
    }
    else if(!regexName.match(name).hasMatch() || !regexName.match(lastName).hasMatch())
    {
        QMessageBox::warning(this, "Warning", "Invalid name or last name!");
        return;
    }
    else if (Password.length() < 8)
    {
        QMessageBox::warning(this, "Warning", "password must be bigger than 7 character!");
        return;
    }
    else if (Password != confirmPass)
    {
        QMessageBox::warning(this, "Warning", "password must be equal with coniform password!");
        return;
    }

    // else db check
    //....


    ui->lineEdit_confirm_pass->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_lastname->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();

    // meesag box

    this->close();
    AplicationManager* am = AplicationManager::instance();
    am->showChoiseWindow();
}

