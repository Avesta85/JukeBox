#ifndef RECEIVESECUREWORDSWINDOW_H
#define RECEIVESECUREWORDSWINDOW_H

#include <QDialog>
#include <Qstring>
#include <QMessageBox>

namespace Ui {
class ReceiveSecureWordsWindow;
}

class ReceiveSecureWordsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveSecureWordsWindow(QWidget *parent = nullptr);
    ~ReceiveSecureWordsWindow();

private slots:
    void on_pushButton_Back_clicked();

    void on_pushButton_coniform_clicked();

private:
    Ui::ReceiveSecureWordsWindow *ui;
};


#endif // RECEIVESECUREWORDSWINDOW_H
