#ifndef ONLINECHATHOST_H
#define ONLINECHATHOST_H

#include <QObject>
#include <QDialog>
#include <QList>
#include <QString>
#include "../backend/core/person.h"

namespace Ui {
class onlineChatHost;
}

class onlineChatHost : public QDialog
{
    Q_OBJECT

public:
    explicit onlineChatHost(QWidget *parent = nullptr);
    ~onlineChatHost();

signals:
    void sendMessage(const QString& msg);
    void endSession();
    void disconnectUsers(const QList<Person>& users);
    void usersDisconnected(const QList<Person>& users);

public slots:
    void onNewMessage(const QString& msg);
    void updateParticipants(const QList<Person>& users);
    void onSessionEnded();
    void onUsersDisconnected(const QList<Person>& users);
    void kickUsers(const QList<Person>& users);

private slots:
    void on_pushButton_Send_2_clicked();
    void on_pushButton_End_clicked();
    void on_pushButton_Disconnect_clicked();

private:
    Ui::onlineChatHost *ui;
};

#endif // ONLINECHATHOST_H

