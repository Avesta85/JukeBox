#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>
#include "aplicationmanager.h"


LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    // 1. فیلد نام کاربری (Username LineEdit)
    // فرض شده نام QLineEdit در UI شما: ui->usernameLineEdit
    // مجاز: حروف انگلیسی (کوچک و بزرگ)، اعداد (0-9)، و آندرسکور (_).
    // حداقل طول: 3 کاراکتر.
    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);
    // به یاد داشته باشید 'usernameLineEdit' را با نام واقعی آبجکت QLineEdit خود جایگزین کنید
    if (ui->lineEdit_username) { // بررسی وجود QLineEdit (روشی خوب)
        ui->lineEdit_username->setValidator(usernameValidator);
    } else {
        qWarning("اخطار: usernameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    // 3. فیلد رمز عبور (Password LineEdit)
    // فرض شده نام QLineEdit در UI شما: ui->passwordLineEdit
    // الزامات:
    //   - حداقل 8 کاراکتر.
    //   - حداقل شامل یک حرف کوچک انگلیسی.
    //   - حداقل شامل یک حرف بزرگ انگلیسی.
    //   - حداقل شامل یک عدد.
    //   - حداقل شامل یک کاراکتر خاص از !@#$%^&*()_+.
    QRegularExpression rxPassword("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*()_+])[A-Za-z\\d!@#$%^&*()_+]{8,}$");
    QRegularExpressionValidator *passwordValidator = new QRegularExpressionValidator(rxPassword, this);
    if (ui->lineEdit_password) {
        ui->lineEdit_password->setValidator(passwordValidator);
        // همچنین خوب است حالت نمایش رمز عبور را تنظیم کنید
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

    // messege...

    // databace...


    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}


void LoginWindow::on_pushButton_cancel_clicked()
{
    AplicationManager* am = AplicationManager::instance();
    am->showChoiseWindow();

    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();
    this->close();
}


void LoginWindow::on_pushButton_forgetpassword_clicked()
{
    AplicationManager* am = AplicationManager::instance();
    am->showForgetPassWindow();

    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();
    this->close();
}

