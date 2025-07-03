#include "online_choice.h"
#include "ui_online_choice.h"

#include <QNetworkInterface>
#include <qhostaddress.h>
#include <qmessagebox.h>
#include <src/backend/core/UserManager.h>
#include <src/backend/network/sessionmanager.h>

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

void online_choice::on_pushButton_Join_clicked()
{
    emit JoinRequest();
}


void online_choice::on_pushButton_Create_clicked()
{
    QString ip;
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : ipAddressesList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress::LocalHost &&
            !address.toString().startsWith("169."))
        {
            ip = address.toString();
            break;
        }
    }
    if (ip.isEmpty())
        ip = QHostAddress(QHostAddress::LocalHost).toString();

    QMessageBox::information(nullptr, "Your IP Address",
                             "Share this IP for others to connect:\n" + ip);

    QHostAddress add(ip);
    SessionManager::getInstance().startNewSession(UserManager::getInstance().getUserName(),add,0);
    emit CreateRequest();
}

