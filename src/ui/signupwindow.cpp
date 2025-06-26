#include "signupwindow.h"
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
    emit backToChoiseWindow();
}


void SignupWindow::on_pushButton_signup_clicked()
{
    QString userNameField = ui->lineEdit_username->text();
    QString nameField = ui->lineEdit_name->text();
    QString lastNameField = ui->lineEdit_lastname->text();
    QString Password = ui->lineEdit_password->text();
    QString email = ui->lineEdit_email->text();
    QString confirmPass = ui->lineEdit_confirm_pass->text();

    QRegularExpression regex("^[a-zA-Z0-9]*$");
    QRegularExpression regexName("^[a-zA-Z]*$");
    QRegularExpression regexEmail("^[a-zA-Z0-9]@email.com$");

    if(userNameField.isEmpty() || nameField.isEmpty() || lastNameField.isEmpty() || Password.isEmpty() || email.isEmpty() || confirmPass.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "All fields must be filled.");
        return;
    }

    else if (!regex.match(userNameField).hasMatch() || !regex.match(nameField).hasMatch() || !regex.match(lastNameField).hasMatch() || !regex.match(Password).hasMatch() || !regex.match(email).hasMatch() || !regex.match(confirmPass).hasMatch())
    {
        QMessageBox::warning(this, "Warning", "Fields can only contain a-z and 0-9.");
        ui->lineEdit_username->clear();
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
    emit backToChoiseWindow();
}

