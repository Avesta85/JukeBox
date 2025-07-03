#include "onlinejoin.h"
#include "src/backend/core/UserManager.h"
#include "ui_onlinejoin.h"

#include <QMessageBox>
#include <QHostAddress>

onlineJoin::onlineJoin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::onlineJoin)
{
    ui->setupUi(this);

}

onlineJoin::~onlineJoin()
{
    delete ui;
}

void onlineJoin::on_pushButton_tryJoin_clicked()
{
    QString ip = ui->lineEdit_HostIp->text().trimmed();
    QHostAddress address(ip);
    if (ip.isEmpty() || address.isNull()) {
        QMessageBox::warning(this, "Join Failed", "Please enter a valid IP address.");
        return;
    }
    emit requestJoin(address , UserManager::getInstance().getUserName());
}

void onlineJoin::joinAccept()
{
    qDebug()<<"joinned to session";
    emit accepted();
}

void onlineJoin::joinReject()
{
    QMessageBox::warning(this, "Join Failed", "Could not join the session. Please check the IP and try again.");
    emit cancel();
}


void onlineJoin::on_pushButton_Cancel_clicked()
{
    emit cancel();
}


