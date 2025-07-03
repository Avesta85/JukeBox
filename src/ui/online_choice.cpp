#include "online_choice.h"
#include "ui_online_choice.h"

online_choice::online_choice(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::online_choice)
{
    ui->setupUi(this);
}

online_choice::~online_choice()
{
    delete ui;
}
