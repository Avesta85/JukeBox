#include "signupwindow.h"
#include "src/backend/core/UserManager.h"
#include "ui_signupwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
{
    ui->setupUi(this);

    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);
    if (ui->lineEdit_username) {
        ui->lineEdit_username->setValidator(usernameValidator);
    } else {
        qWarning("اخطار: usernameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }


    QRegularExpression rxEmail("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
    QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionValidator *emailValidator = new QRegularExpressionValidator(rxEmail, this);
    if (ui->lineEdit_email) {
        ui->lineEdit_email->setValidator(emailValidator);
    } else {
        qWarning("اخطار: emailLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    QRegularExpression rxPassword("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*()_+])[A-Za-z\\d!@#$%^&*()_+]{8,}$");
    QRegularExpressionValidator *passwordValidator = new QRegularExpressionValidator(rxPassword, this);
    if (ui->lineEdit_password) {
        ui->lineEdit_password->setValidator(passwordValidator);
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    } else {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    QRegularExpression rxName("^[\u0621-\u0628\u062A-\u063A\u0641-\u0648\u064E-\u0651\u0655\u067E\u0686\u0698\u06AF\u06CC\\sA-Za-z]{2,}$"); // حروف فارسی، انگلیسی و فاصله
    QRegularExpressionValidator *nameValidator = new QRegularExpressionValidator(rxName, this);
    QRegularExpressionValidator *lastNameValidator = new QRegularExpressionValidator(rxName, this); // استفاده مجدد از همان Regex برای نام خانوادگی

    if (ui->lineEdit_name) {
        ui->lineEdit_name->setValidator(nameValidator);
    } else {
        qWarning("اخطار: nameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
        delete nameValidator;
    }

    if (ui->lineEdit_lastname) {
        ui->lineEdit_lastname->setValidator(lastNameValidator);
    } else {
        qWarning("اخطار: lastNameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
        delete lastNameValidator;
    }

    setWindowIcon(QIcon(":/icone/musicplayer"));
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
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You must fill the fields!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    else if (!regex.match(userName).hasMatch() || !regex.match(Password).hasMatch() || !regex.match(confirmPass).hasMatch())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "User name and password Fields can only contain a-z and 0-9!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }
    else if(!regexEmail.match(email).hasMatch())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You enterd an invalid email!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }
    else if(!regexName.match(name).hasMatch() || !regexName.match(lastName).hasMatch())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You enterd invalid name or last name!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }
    else if (Password.length() < 8)
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "Password must be bigger than 7 character!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }
    else if (Password != confirmPass)
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "Password must be equal with coniform password!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }
    else{
        //gen security key;
        SecurityManager sm;
        auto key = sm.securityKey_gen();
        qDebug()<< key;
        if(UserManager::getInstance().attempSignup(userName,Password,name,lastName,email,key)){

            QMessageBox msgBox(QMessageBox::Warning, "Popup","user signed in", QMessageBox::Ok, this);
            msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
            msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
            msgBox.exec();
            emit showkey(key);

        }
        else{
            QMessageBox msgBox(QMessageBox::Warning, "Warning", "Username already exists", QMessageBox::Ok, this);
            msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
            msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
            msgBox.exec();
        }
    }


    ui->lineEdit_confirm_pass->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_lastname->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();
}

