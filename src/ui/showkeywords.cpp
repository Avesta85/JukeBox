#include "showkeywords.h"
#include "ui_showkeywords.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug> // برای qWarning

ShowKeyWords::ShowKeyWords(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShowKeyWords)
{
    ui->setupUi(this);
}

ShowKeyWords::~ShowKeyWords()
{
    delete ui;
}

void ShowKeyWords::set_key(QString _key)
{
    this->key = _key;
    auto words = key.split('-');

    ui->lineEdit_first->setText(words[0]);
    ui->lineEdit_seconde_word->setText(words[1]);
    ui->lineEdit_third_word->setText(words[2]);
    ui->lineEdit_fourth_word->setText(words[3]);
}

void ShowKeyWords::on_pushButton_clicked()
{
    emit signup_successfully();
}

