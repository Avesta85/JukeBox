#include "receivesecurewordswindow.h"
#include "ui_receivesecurewordswindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>


ReceiveSecureWordsWindow::ReceiveSecureWordsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReceiveSecureWordsWindow)
{
    ui->setupUi(this);

    QRegularExpression rxAlpha("^[\u0621-\u0628\u062A-\u063A\u0641-\u0648\u064E-\u0651\u0655\u067E\u0686\u0698\u06AF\u06CC\\sA-Za-z]{2,}$"); // حروف فارسی، انگلیسی و فاصله
    QRegularExpressionValidator *validatorFirst = new QRegularExpressionValidator(rxAlpha, this);
    if (ui->lineEdit_first) {
        ui->lineEdit_first->setValidator(validatorFirst);
    } else {
        qWarning("اخطار: lineEdit_first در UI یافت نشد. Validator تنظیم نشد.");
        delete validatorFirst;
    }

    QRegularExpressionValidator *validatorSecond = new QRegularExpressionValidator(rxAlpha, this);
    if (ui->lineEdit_seconde_word) {
        ui->lineEdit_seconde_word->setValidator(validatorSecond);
    } else {
        qWarning("اخطار: lineEdit_second در UI یافت نشد. Validator تنظیم نشد.");
        delete validatorSecond;
    }

    QRegularExpressionValidator *validatorThird = new QRegularExpressionValidator(rxAlpha, this);
    if (ui->lineEdit_third_word) {
        ui->lineEdit_third_word->setValidator(validatorThird);
    } else {
        qWarning("اخطار: lineEdit_third در UI یافت نشد. Validator تنظیم نشد.");
        delete validatorThird;
    }

    QRegularExpressionValidator *validatorFourth = new QRegularExpressionValidator(rxAlpha, this);
    if (ui->lineEdit_fourth_word) {
        ui->lineEdit_fourth_word->setValidator(validatorFourth);
    } else {
        qWarning("اخطار: lineEdit_third در UI یافت نشد. Validator تنظیم نشد.");
        delete validatorThird;
    }
}

ReceiveSecureWordsWindow::~ReceiveSecureWordsWindow()
{
    delete ui;
}
