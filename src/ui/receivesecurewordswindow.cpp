#include "receivesecurewordswindow.h"
#include "aplicationmanager.h"
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
    if (ui->lineEdit_first_word) {
        ui->lineEdit_first_word->setValidator(validatorFirst);
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

    setWindowIcon(QIcon(":/icone/musicplayer"));
}

ReceiveSecureWordsWindow::~ReceiveSecureWordsWindow()
{
    delete ui;
}

void ReceiveSecureWordsWindow::on_pushButton_Back_clicked()
{
    ui->lineEdit_first_word->clear();
    ui->lineEdit_seconde_word->clear();
    ui->lineEdit_third_word->clear();
    ui->lineEdit_fourth_word->clear();
    AplicationManager* am = AplicationManager::instance();
    am->showForgetPassWindow();
    this->close();
}


void ReceiveSecureWordsWindow::on_pushButton_coniform_clicked()
{
    QString word1 = ui->lineEdit_first_word->text();
    QString word2 = ui->lineEdit_seconde_word->text();
    QString word3 = ui->lineEdit_third_word->text();
    QString word4 = ui->lineEdit_fourth_word->text();

    if(word1.isEmpty() || word2.isEmpty() || word3.isEmpty() || word4.isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "You must fill the fields!", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();
        return;
    }

    // databace

    ui->lineEdit_first_word->clear();
    ui->lineEdit_seconde_word->clear();
    ui->lineEdit_third_word->clear();
    ui->lineEdit_fourth_word->clear();
    this->close();

    AplicationManager* am = AplicationManager::instance();
    am->showChangePassWindow();
}

