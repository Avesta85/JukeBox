#include "emailverificationwindow.h"
#include "aplicationmanager.h"
#include "ui_emailverificationwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>

EmailVerificationWindow::EmailVerificationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmailVerificationWindow)
{
    ui->setupUi(this);

    // 1. فیلد نام کاربری (Username LineEdit)
    // فرض شده نام QLineEdit در UI شما: ui->usernameLineEdit
    // مجاز: حروف انگلیسی (کوچک و بزرگ)، اعداد (0-9)، و آندرسکور (_).
    // حداقل طول: 3 کاراکتر.
    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);
    // به یاد داشته باشید 'usernameLineEdit' را با نام واقعی آبجکت QLineEdit خود جایگزین کنید
    if (ui->lineEdit_email) { // بررسی وجود QLineEdit (روشی خوب)
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

void EmailVerificationWindow::on_pushButton_Back_clicked()
{
    ui->lineEdit_email->clear();
    AplicationManager* am = AplicationManager::instance();
    am->showForgetPassWindow();
    this->close();
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

    // work with email and data bace..

    ui->lineEdit_email->clear();
    this->close();
    AplicationManager* am = AplicationManager::instance();
    am->showChangePassWindow();
}

