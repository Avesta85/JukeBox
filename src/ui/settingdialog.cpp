#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    // 1. فیلد نام کاربری (Username LineEdit)
    // فرض شده نام QLineEdit در UI شما: ui->usernameLineEdit
    // مجاز: حروف انگلیسی (کوچک و بزرگ)، اعداد (0-9)، و آندرسکور (_).
    // حداقل طول: 3 کاراکتر.
    QRegularExpression rxUsername("^[a-zA-Z0-9_]{3,}$");
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(rxUsername, this);
    // به یاد داشته باشید 'usernameLineEdit' را با نام واقعی آبجکت QLineEdit خود جایگزین کنید
    if (ui->lineEdit_change_username) { // بررسی وجود QLineEdit (روشی خوب)
        ui->lineEdit_change_username->setValidator(usernameValidator);
    } else {
        qWarning("اخطار: usernameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    // 2. فیلد ایمیل (Email LineEdit)
    // فرض شده نام QLineEdit در UI شما: ui->emailLineEdit
    // مجاز: فرمت استاندارد ایمیل. به حروف کوچک و بزرگ حساس نیست (Case-insensitive).
    QRegularExpression rxEmail("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                               QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionValidator *emailValidator = new QRegularExpressionValidator(rxEmail, this);
    if (ui->lineEdit_change_email) {
        ui->lineEdit_change_email->setValidator(emailValidator);
    } else {
        qWarning("اخطار: emailLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    // 4. فیلدهای نام و نام خانوادگی (Name and Last Name LineEdits)
    // فرض شده نام QLineEdit در UI شما: ui->nameLineEdit, ui->lastNameLineEdit
    // مجاز: حروف انگلیسی (کوچک و بزرگ)، حروف فارسی (محدوده اصلی)، و فاصله.
    // حداقل طول: 2 کاراکتر.
    // توجه: پشتیبانی از کاراکترهای فارسی ممکن است نیاز به تنظیمات خاص فونت/Encoding در پروژه شما داشته باشد.
    QRegularExpression rxName("^[\u0621-\u0628\u062A-\u063A\u0641-\u0648\u064E-\u0651\u0655\u067E\u0686\u0698\u06AF\u06CC\\sA-Za-z]{2,}$"); // حروف فارسی، انگلیسی و فاصله
    QRegularExpressionValidator *nameValidator = new QRegularExpressionValidator(rxName, this);
    QRegularExpressionValidator *lastNameValidator = new QRegularExpressionValidator(rxName, this); // استفاده مجدد از همان Regex برای نام خانوادگی

    if (ui->lineEdit_change_name) {
        ui->lineEdit_change_name->setValidator(nameValidator);
    } else {
        qWarning("اخطار: nameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
        // در صورت عدم استفاده، می‌توانید nameValidator را اینجا حذف کنید تا از نشت حافظه جلوگیری شود
        delete nameValidator;
    }
    if (ui->lineEdit_change_lastname) {
        ui->lineEdit_change_lastname->setValidator(lastNameValidator);
    } else {
        qWarning("اخطار: lastNameLineEdit در UI یافت نشد. Validator تنظیم نشد.");
        // در صورت عدم استفاده، می‌توانید lastNameValidator را اینجا حذف کنید
        delete lastNameValidator;
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
    if (ui->lineEdit_current_password) {
        ui->lineEdit_current_password->setValidator(passwordValidator);
        // همچنین خوب است حالت نمایش رمز عبور را تنظیم کنید
        ui->lineEdit_current_password->setEchoMode(QLineEdit::Password);
    } else {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }
    if (ui->lineEdit_new_password) {
        ui->lineEdit_new_password->setValidator(passwordValidator);
        // همچنین خوب است حالت نمایش رمز عبور را تنظیم کنید
        ui->lineEdit_new_password->setEchoMode(QLineEdit::Password);
    } else {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }
    if (ui->lineEdit_confirm_password) {
        ui->lineEdit_confirm_password->setValidator(passwordValidator);
        // همچنین خوب است حالت نمایش رمز عبور را تنظیم کنید
        ui->lineEdit_confirm_password->setEchoMode(QLineEdit::Password);
    } else {
        qWarning("اخطار: passwordLineEdit در UI یافت نشد. Validator تنظیم نشد.");
    }

    setWindowIcon(QIcon(":/icone/musicplayer"));

}

SettingDialog::~SettingDialog()
{
    delete ui;
}
