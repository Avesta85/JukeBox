#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include <QWidget>

namespace Ui {
class ToolBoxWidget;
}

class ToolBoxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBoxWidget(QWidget *parent = nullptr);
    ~ToolBoxWidget();

private:
    Ui::ToolBoxWidget *ui;
};

#endif // TOOLBOXWIDGET_H
