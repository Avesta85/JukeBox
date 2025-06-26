#ifndef FROGOTPASSWORDWINDOW_H
#define FROGOTPASSWORDWINDOW_H

#include <QDialog>
#include <QString>
#include <QRegularExpression>
#include <QMessageBox>
#include "emailverificationwindow.h"
#include "receivesecurewordswindow.h"

namespace Ui {
class FrogotPasswordWindow;
}

class FrogotPasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FrogotPasswordWindow(QWidget *parent = nullptr);
    ~FrogotPasswordWindow();

signals:
    void backToLoginWindow();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::FrogotPasswordWindow *ui;
    EmailVerificationWindow *emailVrifi;
    ReceiveSecureWordsWindow *secureWordsVrifi;

};

#endif // FROGOTPASSWORDWINDOW_H
