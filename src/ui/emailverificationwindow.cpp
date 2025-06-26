#include "emailverificationwindow.h"
#include "ui_emailverificationwindow.h"

EmailVerificationWindow::EmailVerificationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmailVerificationWindow)
{
    ui->setupUi(this);
    changePassWindow = new ChangePasswordWindow;
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


void EmailVerificationWindow::on_pushButton_coniform_clicked()
{
    QString vrifiCode = ui->lineEdit_email->text();

    if(vrifiCode.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "You must fill the field!");
        return;
    }

    // work with email and data bace..

    ui->lineEdit_email->clear();
    this->close();
    changePassWindow->show();
}

