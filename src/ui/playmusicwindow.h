#ifndef PLAYMUSICWINDOW_H
#define PLAYMUSICWINDOW_H

#include <QDialog>

namespace Ui {
class playmusicwindow;
}

class playmusicwindow : public QDialog
{
    Q_OBJECT

public:
    explicit playmusicwindow(QWidget *parent = nullptr);
    ~playmusicwindow();

private:
    Ui::playmusicwindow *ui;
};

#endif // PLAYMUSICWINDOW_H
