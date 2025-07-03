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

private:
    Ui::online_choice *ui;
};

#endif // ONLINE_CHOICE_H
