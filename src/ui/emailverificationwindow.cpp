#include "emailverificationwindow.h"
#include "aplicationmanager.h"
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
    AplicationManager* am = AplicationManager::instance();
    am->showForgetPassWindow();
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
    AplicationManager* am = AplicationManager::instance();
    am->showChangePassWindow();
}

