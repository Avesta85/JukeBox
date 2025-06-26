#include "emailverificationwindow.h"
#include "ui_emailverificationwindow.h"

EmailVerificationWindow::EmailVerificationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmailVerificationWindow)
{
    ui->setupUi(this);
}

EmailVerificationWindow::~EmailVerificationWindow()
{
    delete ui;
}

void EmailVerificationWindow::on_pushButton_Back_clicked()
{
    ui->lineEdit_email->clear();
    emit backToForgetPassWindow();
    this->close();
}

