#include "playmusicwindow.h"
#include "ui_playmusicwindow.h"

playmusicwindow::playmusicwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::playmusicwindow)
{
    ui->setupUi(this);
}

playmusicwindow::~playmusicwindow()
{
    delete ui;
}
