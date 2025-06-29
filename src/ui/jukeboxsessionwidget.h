#ifndef JUKEBOXSESSIONWIDGET_H
#define JUKEBOXSESSIONWIDGET_H

#include <QDockWidget>

namespace Ui {
class JukeBoxSessionWidget;
}

class JukeBoxSessionWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit JukeBoxSessionWidget(QWidget *parent = nullptr);
    ~JukeBoxSessionWidget();

private:
    Ui::JukeBoxSessionWidget *ui;
};

#endif // JUKEBOXSESSIONWIDGET_H
