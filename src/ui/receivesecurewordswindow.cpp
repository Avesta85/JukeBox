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
