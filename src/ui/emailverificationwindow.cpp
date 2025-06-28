#include "emailverificationwindow.h"
#include "src/backend/security/SecurityManager.h"
#include "ui_emailverificationwindow.h"

EmailVerificationWindow::EmailVerificationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmailVerificationWindow)
{
    ui->setupUi(this);
    changePassWindow = new ChangePasswordWindow;
}

EmailVerificationWindow::~EmailVerificationWindow()
{
    delete ui;
}

void EmailVerificationWindow::Emailsender(QString email)
{
    SecurityManager sm;
    try{

        sended_email = sm.SendVerification_Code(email);
    }
    catch(...)
    {
        sended_email = "...";
        QMessageBox::warning(this, "Warning", "email not sended.");
    }
}

void EmailVerificationWindow::setResiverUsername(QString username)
{
    this ->reciver_username = username;
}

void EmailVerificationWindow::on_pushButton_Back_clicked()
{
    ui->lineEdit_email->clear();
    emit backToForgetPassWindow();
}


void EmailVerificationWindow::on_pushButton_coniform_clicked()
{
    QString vrifiCode = ui->lineEdit_email->text();

    if(vrifiCode.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "You must fill the field!");
        return;
    }
    if(vrifiCode == sended_email && sended_email !="..."){

        emit passVerification_gotoChangepassword(this->reciver_username);
        ui->lineEdit_email->clear();
    }
    else{
        QMessageBox::warning(this, "Warning", "code is incorrect");
        emit backToForgetPassWindow();
    }
}

