#include "createdialog.h"
#include "ui_createdialog.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include "src/backend/core/song.h"
#include <QDir>
#include <QListWidgetItem>

mycreateDialog::mycreateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::mycreateDialog)
{
    ui->setupUi(this);
}

mycreateDialog::~mycreateDialog()
{
    delete ui;
}

void mycreateDialog::on_pushButton_creat_clicked()
{
    QString name = ui->lineEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Empty Name", "Please enter a name for the playlist.");
        return;
    }
    QList<qint64> selectedSongIds;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        if (item && item->checkState() == Qt::Checked) {
            selectedSongIds.append(item->data(Qt::UserRole).toLongLong());
        }
    }
    emit CreatePlaylist(name, selectedSongIds);
    this->accept();
}

void mycreateDialog::update_songsList(const QList<Song> songsList)
{
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Song Name");

    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < songsList.size(); ++i) {
        ui->tableWidget->insertRow(i);
        QTableWidgetItem* item = new QTableWidgetItem(songsList[i].getName());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setData(Qt::UserRole, QVariant::fromValue<qint64>(songsList[i].getID()));
        ui->tableWidget->setItem(i, 0, item);
    }
}




