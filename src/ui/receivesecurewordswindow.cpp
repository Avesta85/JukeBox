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

void ReceiveSecureWordsWindow::setusername(QString username)
{
    this ->username = username;
}

void ReceiveSecureWordsWindow::setkey(QString key)
{
    this ->key = key;
}

void ReceiveSecureWordsWindow::on_pushButton_Back_clicked()
{
    ui->lineEdit_first_word->clear();
    ui->lineEdit_seconde_word->clear();
    ui->lineEdit_third_word->clear();
    ui->lineEdit_fourth_word->clear();
    emit backToForgetPassWindow();

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

    auto words = this->key.split('-');
    try{

        if(
            word1 !=words[0] ||
            word2 !=words[1] ||
            word3 !=words[2] ||
            word4 !=words[3]
            )
        {
            QMessageBox::warning(this, "Warning", "Security key incorrect.");
            emit backToForgetPassWindow();
        }
        else{
            emit passed(this->username);

            ui->lineEdit_first_word->clear();
            ui->lineEdit_seconde_word->clear();
            ui->lineEdit_third_word->clear();
            ui->lineEdit_fourth_word->clear();

        }
    }
    catch(...)
    {
        QMessageBox::warning(this, "Warning", "operation failed.");
        emit backToForgetPassWindow();
    }
}

