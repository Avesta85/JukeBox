#include "emailverificationwindow.h"
#include "src/backend/security/SecurityManager.h"
#include "ui_emailverificationwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

EmailVerificationWindow::EmailVerificationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmailVerificationWindow)
{
    ui->setupUi(this);
    changePassWindow = new ChangePasswordWindow;

    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);

    if (ui->lineEdit_email) {
        ui->lineEdit_email->setValidator(usernameValidator);
    } else {
        qWarning("اخطار: usernameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    setWindowIcon(QIcon(":/icone/musicplayer"));
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
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You must fill the field!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    if(vrifiCode == sended_email && sended_email !="..."){

        emit passVerification_gotoChangepassword(this->reciver_username);
        ui->lineEdit_email->clear();
    }
    else{
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "code is incorrect", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        emit backToForgetPassWindow();
    }
}

