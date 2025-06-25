#include "frogotpasswordwindow.h"
#include "ui_frogotpasswordwindow.h"

FrogotPasswordWindow::FrogotPasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FrogotPasswordWindow)
{
    ui->setupUi(this);
}

FrogotPasswordWindow::~FrogotPasswordWindow()
{
    delete ui;
}
