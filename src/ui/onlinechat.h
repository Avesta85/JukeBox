#ifndef ONLINECHAT_H
#define ONLINECHAT_H

#include <QDialog>

namespace Ui {
class onlinechat;
}

class onlinechat : public QDialog
{
    Q_OBJECT

public:
    explicit onlinechat(QWidget *parent = nullptr);
    ~onlinechat();

signals:
    void sendMessage(const QString& msg);
    void leaveSession();
    void clientKicked(const QString& reason);

public slots:
    void onNewMessage(const QString& msg);
    void updateParticipants(const QStringList& users);
    void onSessionEnded();
    void onSystemMessage(const QString& msg);
    void onKicked(const QString& reason);

private slots:
    void on_pushButton_send_clicked();
    void on_pushButton_leave_clicked();
    void on_lineEdit_message_returnPressed();

private:
    Ui::onlinechat *ui;
};

#endif // ONLINECHAT_H

