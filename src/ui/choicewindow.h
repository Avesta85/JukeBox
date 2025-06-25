#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H

#include <QDialog>

namespace Ui {
class ChoiceWindow;
}

class ChoiceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChoiceWindow(QWidget *parent = nullptr);
    ~ChoiceWindow();

private:
    Ui::ChoiceWindow *ui;
};

#endif // CHOICEWINDOW_H
