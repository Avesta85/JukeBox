#include "receivesecurewordswindow.h"
#include "ui_receivesecurewordswindow.h"

ReceiveSecureWordsWindow::ReceiveSecureWordsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReceiveSecureWordsWindow)
{
    ui->setupUi(this);
    changePassWidnow = new ChangePasswordWindow;
}

ReceiveSecureWordsWindow::~ReceiveSecureWordsWindow()
{
    delete ui;
    delete changePassWidnow;
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


void ReceiveSecureWordsWindow::on_pushButton_coniform_clicked()
{
    QString word1 = ui->lineEdit_first_word->text();
    QString word2 = ui->lineEdit_seconde_word->text();
    QString word3 = ui->lineEdit_third_word->text();
    QString word4 = ui->lineEdit_fourth_word->text();

    if(word1.isEmpty() || word2.isEmpty() || word3.isEmpty() || word4.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "You must fill the fields!");
        return;
    }

    // databace

    ui->lineEdit_first_word->clear();
    ui->lineEdit_seconde_word->clear();
    ui->lineEdit_third_word->clear();
    ui->lineEdit_fourth_word->clear();
    this->close();

    changePassWidnow->show();
}

