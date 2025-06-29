#include "moviemanagementwindow.h"
#include "ui_moviemanagementwindow.h"

MovieManagementWindow::MovieManagementWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MovieManagementWindow)
{
    ui->setupUi(this);
}

MovieManagementWindow::~MovieManagementWindow()
{
    delete ui;
}
