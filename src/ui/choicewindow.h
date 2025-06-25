#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H

#include <QDialog>

namespace Ui {
class Choicewindow;
}

class Choicewindow : public QDialog
{
    Q_OBJECT

public:
    explicit Choicewindow(QWidget *parent = nullptr);
    ~Choicewindow();

private:
    Ui::Choicewindow *ui;
};

#endif // CHOICEWINDOW_H
