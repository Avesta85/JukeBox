#ifndef ONLINE_CHOICE_H
#define ONLINE_CHOICE_H

#include <QDialog>

namespace Ui {
class online_choice;
}

class online_choice : public QDialog
{
    Q_OBJECT

public:
    explicit online_choice(QWidget *parent = nullptr);
    ~online_choice();



signals:
    void JoinRequest();
    void CreateRequest();
private slots:
    void on_pushButton_Join_clicked();

    void on_pushButton_Create_clicked();

private:
    Ui::online_choice *ui;
};

#endif // ONLINE_CHOICE_H

