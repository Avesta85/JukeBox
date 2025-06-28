#ifndef FROGOTPASSWORDWINDOW_H
#define FROGOTPASSWORDWINDOW_H

#include <QDialog>
#include <QString>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDir>
#include <QIcon>

namespace Ui {
class FrogotPasswordWindow;
}

class FrogotPasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FrogotPasswordWindow(QWidget *parent = nullptr);
    ~FrogotPasswordWindow();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_back_clicked();

private:
    Ui::FrogotPasswordWindow *ui;
};

#endif // FROGOTPASSWORDWINDOW_H
