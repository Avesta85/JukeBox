#ifndef EMAILVERIFICATIONWINDOW_H
#define EMAILVERIFICATIONWINDOW_H
#include <QString>
#include <QMessageBox>
#include <QDialog>

namespace Ui {
class EmailVerificationWindow;
}

class EmailVerificationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmailVerificationWindow(QWidget *parent = nullptr);
    ~EmailVerificationWindow();


private slots:
    void on_pushButton_Back_clicked();
    void on_pushButton_coniform_clicked();

private:
    Ui::EmailVerificationWindow *ui;

};

#endif // EMAILVERIFICATIONWINDOW_H
