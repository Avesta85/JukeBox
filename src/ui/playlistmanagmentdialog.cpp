#include "playlistmanagmentdialog.h"
#include "ui_playlistmanagmentdialog.h"

PlayListManagmentDialog::PlayListManagmentDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlayListManagmentDialog)
{
    ui->setupUi(this);
}

PlayListManagmentDialog::~PlayListManagmentDialog()
{
    delete ui;
}
