#include "onlinechathost.h"
#include "ui_onlinechathost.h"
#include <QListWidgetItem>

onlineChatHost::onlineChatHost(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::onlineChatHost)
{
    ui->setupUi(this);
    connect(ui->pushButton_Send_2, &QPushButton::clicked, this, &onlineChatHost::on_pushButton_Send_2_clicked);
    connect(ui->pushButton_End, &QPushButton::clicked, this, &onlineChatHost::on_pushButton_End_clicked);
    connect(ui->pushButton_Disconnect, &QPushButton::clicked, this, &onlineChatHost::on_pushButton_Disconnect_clicked);
}

onlineChatHost::~onlineChatHost()
{
    delete ui;
}

void onlineChatHost::on_pushButton_Send_2_clicked()
{
    QString msg = ui->lineEdit_message_2->text().trimmed();
    if (!msg.isEmpty()) {
        emit sendMessage(msg);
        ui->lineEdit_message_2->clear();
    }
}

void onlineChatHost::on_pushButton_End_clicked()
{
    emit endSession();
}

void onlineChatHost::on_pushButton_Disconnect_clicked()
{
    QList<Person> selectedUsers;
    for (int i = 0; i < ui->listWidget_Participant_2->count(); ++i) {
        QListWidgetItem* item = ui->listWidget_Participant_2->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedUsers.append(Person(item->text()));
        }
    }
    if (!selectedUsers.isEmpty())
        emit disconnectUsers(selectedUsers);
}

void onlineChatHost::onNewMessage(const QString& msg)
{
    ui->textEdit_allChats_2->append(msg);
}

void onlineChatHost::updateParticipants(const QList<Person>& users)
{
    ui->listWidget_Participant_2->clear();
    for (const Person& user : users) {
        QListWidgetItem* item = new QListWidgetItem(user.getUserName());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget_Participant_2->addItem(item);
    }
}

void onlineChatHost::onSessionEnded()
{
    ui->textEdit_allChats_2->append("Session ended.");
    this->close();
}

void onlineChatHost::onUsersDisconnected(const QList<Person>& users)
{
    for (const Person& user : users) {
        QString username = user.getUserName();
        QList<QListWidgetItem*> found = ui->listWidget_Participant_2->findItems(username, Qt::MatchExactly);
        for (QListWidgetItem* item : found) {
            delete ui->listWidget_Participant_2->takeItem(ui->listWidget_Participant_2->row(item));
        }
    }
}

void onlineChatHost::kickUsers(const QList<Person>& users)
{
    // این اسلات می‌تواند از UI یا SessionManager فراخوانی شود
    // برای اخراج کاربران از سشن
    emit disconnectUsers(users);
    
    // به‌روزرسانی UI
    for (const Person& user : users) {
        QString username = user.getUserName();
        QList<QListWidgetItem*> found = ui->listWidget_Participant_2->findItems(username, Qt::MatchExactly);
        for (QListWidgetItem* item : found) {
            delete ui->listWidget_Participant_2->takeItem(ui->listWidget_Participant_2->row(item));
        }
    }
}
