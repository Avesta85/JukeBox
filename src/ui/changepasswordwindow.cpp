#include "changepasswordwindow.h"
#include "aplicationmanager.h"
#include "ui_changepasswordwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>

ChangePasswordWindow::ChangePasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordWindow)
{
    ui->setupUi(this);

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
        ui->lineEdit_confirmePassword->setEchoMode(QLineEdit::Password);
    } else {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    QRegularExpressionValidator *passwordValidator1 = new QRegularExpressionValidator(rxPassword, this);
    if (ui->lineEdit_confirmePassword) {
        ui->lineEdit_confirmePassword->setValidator(passwordValidator1);
        // همچنین خوب است حالت نمایش رمز عبور را تنظیم کنید
        ui->lineEdit_confirmePassword->setEchoMode(QLineEdit::Password);
    } else {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }
}

ChangePasswordWindow::~ChangePasswordWindow()
{
    delete ui;
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

    ui->lineEdit_password->clear();
    ui->lineEdit_confirmePassword->clear();

    this->close();
    AplicationManager* am = AplicationManager::instance();
    am->showLoginWindow();
}

