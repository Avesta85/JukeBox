#include "changepasswordwindow.h"
#include "src/backend/db/DBM.h"
#include "src/backend/security/SecurityManager.h"
#include "ui_changepasswordwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

ChangePasswordWindow::ChangePasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordWindow)
{
    ui->setupUi(this);

    QRegularExpression rxPassword("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*()_+])[A-Za-z\\d!@#$%^&*()_+]{8,}$");
    QRegularExpressionValidator *passwordValidator = new QRegularExpressionValidator(rxPassword, this);
    if (ui->lineEdit_password) {
        ui->lineEdit_password->setValidator(passwordValidator);
        // همچنین خوب است حالت نمایش رمز عبور را تنظیم کنید
        ui->lineEdit_confirmePassword->setEchoMode(QLineEdit::Password);
    } else
    {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    QRegularExpressionValidator *passwordValidator1 = new QRegularExpressionValidator(rxPassword, this);
    if (ui->lineEdit_confirmePassword) {
        ui->lineEdit_confirmePassword->setValidator(passwordValidator1);
        // همچنین خوب است حالت نمایش رمز عبور را تنظیم کنید
        ui->lineEdit_confirmePassword->setEchoMode(QLineEdit::Password);
    } else
    {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    setWindowIcon(QIcon(":/icone/musicplayer"));
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
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You must fill the field!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }
    else if(password != confirmPassword)
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "Password must bu equal with confirm password!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
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

