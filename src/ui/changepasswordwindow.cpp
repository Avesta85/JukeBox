#include "changepasswordwindow.h"
#include "ui_changepasswordwindow.h"

ChangePasswordWindow::ChangePasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordWindow)
{
    ui->setupUi(this);
}

ChangePasswordWindow::~ChangePasswordWindow()
{
    delete ui;
}
