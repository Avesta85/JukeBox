#include "frogotpasswordwindow.h"
#include "src/backend/db/DBM.h"
#include "ui_frogotpasswordwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

FrogotPasswordWindow::FrogotPasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FrogotPasswordWindow)
{
    ui->setupUi(this);

    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);

    if (ui->lineEdit) {
        ui->lineEdit->setValidator(usernameValidator);
    } else {
        qWarning("اخطار: usernameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    setWindowIcon(QIcon(":/icone/musicplayer"));
}

FrogotPasswordWindow::~FrogotPasswordWindow()
{
    delete ui;

}

void FrogotPasswordWindow::on_pushButton_ok_clicked()
{
    QString userNameField = ui->lineEdit->text();
    QRegularExpression regex("^[a-zA-Z0-9]*$");
    QString email;
    if(userNameField.isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "Please fill the field of user name!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    else if (!regex.match(userNameField).hasMatch())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You enterd an invalid user name!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        ui->lineEdit->clear();
        return;
    }

    else if (!ui->radioButton_email_verification->isChecked() && !ui->radioButton_secure_words->isChecked())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You must choice a option!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    // databace

    if(ui->radioButton_email_verification->isChecked())
    {
        if((email=DBM::get_instance().getEmailofUser(userNameField)) ==""){
            QMessageBox msgBox(QMessageBox::Warning, "Warning", "Please Enter Valid Username.", QMessageBox::Ok, this);
            msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
            msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
            msgBox.exec();
            return;
        }
        emit EmailVerification(email,userNameField);
        ui->radioButton_email_verification->setChecked(false);
        ui->lineEdit->clear();


    }

    else if(ui->radioButton_secure_words->isChecked())
    {
        QString key =DBM::get_instance().getSKeyofUser(userNameField);
        if(key == ""){
            QMessageBox msgBox(QMessageBox::Warning, "Warning", "Please Enter Valid Username.", QMessageBox::Ok, this);
            msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
            msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
            msgBox.exec();
            return;
        }

        emit SecurityVerification(key,userNameField);
        ui->radioButton_secure_words->setChecked(true);
        ui->lineEdit->clear();

    }

}


void FrogotPasswordWindow::on_pushButton_back_clicked()
{
    emit backToLoginWindow();
    ui->lineEdit->clear();
}

