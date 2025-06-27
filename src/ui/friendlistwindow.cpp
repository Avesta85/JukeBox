#include "friendlistwindow.h"
#include "ui_friendlistwindow.h"

FriendListWindow::FriendListWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FriendListWindow)
{
    ui->setupUi(this);
}

FriendListWindow::~FriendListWindow()
{
    delete ui;
}
