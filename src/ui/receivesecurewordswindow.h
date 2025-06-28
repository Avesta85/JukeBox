#ifndef RECEIVESECUREWORDSWINDOW_H
#define RECEIVESECUREWORDSWINDOW_H

#include <QDialog>
#include <Qstring>
#include <QMessageBox>
#include "changepasswordwindow.h"

namespace Ui {
class ReceiveSecureWordsWindow;
}

class ReceiveSecureWordsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveSecureWordsWindow(QWidget *parent = nullptr);
    ~ReceiveSecureWordsWindow();

    void setusername(QString username);
    void setkey(QString key);

signals:
    void backToForgetPassWindow();
    void passed(QString username);

private slots:
    void on_pushButton_Back_clicked();

    void on_pushButton_coniform_clicked();

private:
    Ui::ReceiveSecureWordsWindow *ui;
    ChangePasswordWindow* changePassWidnow;
    QString username;
    QString key;
};


#endif // RECEIVESECUREWORDSWINDOW_H
