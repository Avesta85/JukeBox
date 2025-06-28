#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H

#include <QStackedWidget>

namespace Ui {
class StageWidget;
}

class StageWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit StageWidget(QWidget *parent = nullptr);
    ~StageWidget();

private:
    Ui::StageWidget *ui;
};

#endif // STAGEWIDGET_H
