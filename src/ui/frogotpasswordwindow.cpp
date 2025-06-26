#include "frogotpasswordwindow.h"
#include "ui_frogotpasswordwindow.h"

FrogotPasswordWindow::FrogotPasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FrogotPasswordWindow)
{
    ui->setupUi(this);
    emailVrifi = new EmailVerificationWindow;
    secureWordsVrifi = new ReceiveSecureWordsWindow;
    connect(emailVrifi, &EmailVerificationWindow::backToForgetPassWindow, this, &FrogotPasswordWindow::show);
    connect(secureWordsVrifi , &ReceiveSecureWordsWindow::backToForgetPassWindow, this, &FrogotPasswordWindow::show);
}

FrogotPasswordWindow::~FrogotPasswordWindow()
{
    delete ui;
    delete emailVrifi;
    delete secureWordsVrifi;
}

void FrogotPasswordWindow::on_pushButton_ok_clicked()
{
    QString userNameField = ui->lineEdit->text();
    QRegularExpression regex("^[a-zA-Z0-9]*$");

    if(userNameField.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please fill the field of user name");
        return;
    }

    else if (!regex.match(userNameField).hasMatch())
    {
        QMessageBox::warning(this, "Warning", "Field of user name can only contain a-z and 0-9.");
        ui->lineEdit->clear();
        return;
    }

    else if (!ui->radioButton_email_verification->isChecked() && !ui->radioButton_secure_words->isChecked())
    {
        QMessageBox::warning(this, "Warning", "Please select one option.");
        return;
    }

    // else unmach with databace

    // databace

    if(ui->radioButton_email_verification->isChecked())
    {
        ui->radioButton_email_verification->setChecked(false);
        ui->lineEdit->clear();
        this->close();
        emailVrifi->show();

    }

    else if(ui->radioButton_secure_words->isChecked())
    {
        ui->radioButton_secure_words->setChecked(true);
        ui->lineEdit->clear();
        this->close();
        secureWordsVrifi->show();
    }

}


void FrogotPasswordWindow::on_pushButton_back_clicked()
{
    emit backToLoginWindow();
    ui->lineEdit->clear();
    this->close();
}

