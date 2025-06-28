#ifndef FROGOTPASSWORDWINDOW_H
#define FROGOTPASSWORDWINDOW_H

#include <QDialog>
#include <QString>
#include <QRegularExpression>
#include <QMessageBox>

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
    void EmailVerification(QString Email,QString username);
    void SecurityVerification(QString key,QString username);
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::FrogotPasswordWindow *ui;


};

#endif // FROGOTPASSWORDWINDOW_H
