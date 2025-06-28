#include "frogotpasswordwindow.h"
#include "aplicationmanager.h"
#include "ui_frogotpasswordwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>

FrogotPasswordWindow::FrogotPasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FrogotPasswordWindow)
{
    ui->setupUi(this);

    // 1. فیلد نام کاربری (Username LineEdit)
    // فرض شده نام QLineEdit در UI شما: ui->usernameLineEdit
    // مجاز: حروف انگلیسی (کوچک و بزرگ)، اعداد (0-9)، و آندرسکور (_).
    // حداقل طول: 3 کاراکتر.
    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);
    // به یاد داشته باشید 'usernameLineEdit' را با نام واقعی آبجکت QLineEdit خود جایگزین کنید
    if (ui->lineEdit) { // بررسی وجود QLineEdit (روشی خوب)
        ui->lineEdit->setValidator(usernameValidator);
    } else {
        qWarning("اخطار: usernameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }
}

FrogotPasswordWindow::~FrogotPasswordWindow()
{
    delete ui;
}

void FrogotPasswordWindow::on_pushButton_ok_clicked()
{
    QString userNameField = ui->lineEdit->text();
    QRegularExpression regex("^[a-zA-Z0-9]*$");

    if(userNameField.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please fill the field of user name");
        return;
    }

    else if (!regex.match(userNameField).hasMatch())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "kkkk.", QMessageBox::Ok, this);
        msgBox.setIconPixmap(QPixmap(":/warning.png"));
        msgBox.exec();
        ui->lineEdit->clear();
        return;
    }

    else if (!ui->radioButton_email_verification->isChecked() && !ui->radioButton_secure_words->isChecked())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "kkkkkkkkkkkk.", QMessageBox::Ok, this);
       // msgBox.setIconPixmap(QPixmap(":/icone/warning.png"));
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.exec();
        ui->lineEdit->clear();
        return;
    }

    // else unmach with databace

    // databace

    if(ui->radioButton_email_verification->isChecked())
    {
        ui->radioButton_email_verification->setChecked(false);
        ui->lineEdit->clear();
        AplicationManager* am = AplicationManager::instance();
        am->showEmailVrifiWindow();
        this->close();
    }

    else if(ui->radioButton_secure_words->isChecked())
    {
        ui->radioButton_secure_words->setChecked(true);
        ui->lineEdit->clear();
        AplicationManager* am = AplicationManager::instance();
        am->showSecureWordWindow();
        this->close();
    }

}


void FrogotPasswordWindow::on_pushButton_back_clicked()
{
    AplicationManager* am = AplicationManager::instance();
    am->showLoginWindow();
    ui->lineEdit->clear();
    this->close();
}

