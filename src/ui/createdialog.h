#ifndef CREATEDIALOG_H
#define CREATEDIALOG_H

#include <QDialog>
#include <QDir>
#include <QListWidgetItem>

namespace Ui {
class CreateDialog;
}

class CreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDialog(QWidget *parent = nullptr);
    ~CreateDialog();

private slots:
    void on_pushButton_creat_clicked();

private:
    Ui::CreateDialog *ui;
};

#endif // CREATEDIALOG_H
