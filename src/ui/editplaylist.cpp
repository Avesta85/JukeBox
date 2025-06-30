#include "editplaylist.h"
#include "ui_editplaylist.h"

EditPlayList::EditPlayList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditPlayList)
{
    ui->setupUi(this);
}

EditPlayList::~EditPlayList()
{
    delete ui;
}
