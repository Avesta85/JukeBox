#ifndef PLAYERCONTROLWIDGET_H
#define PLAYERCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class PlayerControlWidget;
}

class PlayerControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControlWidget(QWidget *parent = nullptr);
    ~PlayerControlWidget();

private:
    Ui::PlayerControlWidget *ui;
};

#endif // PLAYERCONTROLWIDGET_H
