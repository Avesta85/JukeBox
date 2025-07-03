#ifndef ONLINEJOIN_H
#define ONLINEJOIN_H

#include "qhostaddress.h"
#include <QDialog>

namespace Ui {
class onlineJoin;
}

class onlineJoin : public QDialog
{
    Q_OBJECT

public:
    explicit onlineJoin(QWidget *parent = nullptr);
    ~onlineJoin();


public slots:
    void joinAccept();
    void joinReject();

signals:
    void requestJoin(const QHostAddress Hostip,const QString username);
    void cancel();
    void accepted();
private slots:
    void on_pushButton_tryJoin_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::onlineJoin *ui;
};

#endif // ONLINEJOIN_H

