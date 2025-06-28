#ifndef CHANGEPASSWORDWINDOW_H
#define CHANGEPASSWORDWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QString>

namespace Ui {
class ChangePasswordWindow;
}

class ChangePasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordWindow(QWidget *parent = nullptr);
    ~ChangePasswordWindow();

    void set_username(QString Username);
private slots:
    void on_pushButton_clicked();

signals:
    void goToLoginWindow();

private:
    Ui::ChangePasswordWindow *ui;
    QString Username;
};

#endif // CHANGEPASSWORDWINDOW_H
