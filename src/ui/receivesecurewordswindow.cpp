#include "receivesecurewordswindow.h"
#include "ui_receivesecurewordswindow.h"

ReceiveSecureWordsWindow::ReceiveSecureWordsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReceiveSecureWordsWindow)
{
    ui->setupUi(this);
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
    emit backToForgetPassWindow();
    this->close();
}

