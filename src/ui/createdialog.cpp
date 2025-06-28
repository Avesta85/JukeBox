#include "createdialog.h"
#include "ui_createdialog.h"


CreateDialog::CreateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateDialog)
{
    ui->setupUi(this);

    QDir dir("path music folder");
    QStringList audioFiles = dir.entryList(QStringList() << "*.mp3" << "*.wav", QDir::Files);

    foreach (const QString &fileName, audioFiles) {
        QListWidgetItem *item = new QListWidgetItem(fileName);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
    }
}

CreateDialog::~CreateDialog()
{
    delete ui;
}

void CreateDialog::on_pushButton_creat_clicked()
{

}

