#include "onlinechat.h"
#include "ui_onlinechat.h"
#include <QTimer>

onlinechat::onlinechat(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::onlinechat)
{
    ui->setupUi(this);
    connect(ui->pushButton_Send, &QPushButton::clicked, this, &onlinechat::on_pushButton_send_clicked);
    connect(ui->pushButton_Leave, &QPushButton::clicked, this, &onlinechat::on_pushButton_leave_clicked);
    connect(ui->lineEdit_message, &QLineEdit::returnPressed, this, &onlinechat::on_lineEdit_message_returnPressed);
}

onlinechat::~onlinechat()
{
    delete ui;
}

void onlinechat::on_pushButton_send_clicked()
{
    QString msg = ui->lineEdit_message->text().trimmed();
    if (!msg.isEmpty()) {
        emit sendMessage(msg);
        ui->lineEdit_message->clear();
    }
}

void onlinechat::on_pushButton_leave_clicked()
{
    emit leaveSession();
}

void onlinechat::onNewMessage(const QString& msg)
{
    ui->textEdit_allChats->append(msg);
}

void onlinechat::updateParticipants(const QStringList& users)
{
    ui->listWidget_Participant->clear();
    ui->listWidget_Participant->addItems(users);
}

void onlinechat::on_lineEdit_message_returnPressed()
{
    on_pushButton_send_clicked();
}

void onlinechat::onSystemMessage(const QString& msg)
{
    ui->textEdit_allChats->append(QString("<i>%1</i>").arg(msg));
}

void onlinechat::onKicked(const QString& reason)
{
    ui->textEdit_allChats->append(QString("<b>You have been kicked from the session.</b> Reason: %1").arg(reason));
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_Leave->setEnabled(false);
    ui->lineEdit_message->setEnabled(false);
    ui->listWidget_Participant->setEnabled(false);
    // optionally close after a delay
    QTimer::singleShot(2000, this, &QDialog::close);
}

void onlinechat::onSessionEnded()
{
    ui->textEdit_allChats->append("<b>Session ended.</b>");
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_Leave->setEnabled(false);
    ui->lineEdit_message->setEnabled(false);
    ui->listWidget_Participant->setEnabled(false);
    QTimer::singleShot(2000, this, &QDialog::close);
}
