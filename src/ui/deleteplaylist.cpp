#include "deleteplaylist.h"
#include "ui_deleteplaylist.h"

DeletePlayList::DeletePlayList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeletePlayList)
{
    ui->setupUi(this);
}

DeletePlayList::~DeletePlayList()
{
    delete ui;
}

void DeletePlayList::on_pushButton_clicked()
{

}

