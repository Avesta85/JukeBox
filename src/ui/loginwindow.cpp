#include "loginwindow.h"
#include "src/backend/core/UserManager.h"
#include "ui_loginwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);

    if (ui->lineEdit_username) {
        ui->lineEdit_username->setValidator(usernameValidator);
    } else {
        qWarning("اخطار: usernameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    QRegularExpression rxPassword("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*()_+])[A-Za-z\\d!@#$%^&*()_+]{8,}$");
    QRegularExpressionValidator *passwordValidator = new QRegularExpressionValidator(rxPassword, this);
    if (ui->lineEdit_password) {
        ui->lineEdit_password->setValidator(passwordValidator);

        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    } else {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    setWindowIcon(QIcon(":/icone/musicplayer"));
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
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "All fields must be filled!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    else if (!regex.match(userNameField).hasMatch() || !regex.match(passField).hasMatch())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "Fields can only contain a-z and 0-9!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        ui->lineEdit_username->clear();
        return;
    }

    else if(passField.length()<8)
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "Password must be begger than 7 charecter!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    if(!UserManager::getInstance().attemptLogin(userNameField,passField)){
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "operation failed.", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    if(!UserManager::getInstance().is_loggedin()){
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "password or username is incurrect.", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }
    else{
        emit LoggedInsuccessfully();
    }

    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
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

