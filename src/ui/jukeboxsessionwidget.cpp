#include "jukeboxsessionwidget.h"
#include "ui_jukeboxsessionwidget.h"

JukeBoxSessionWidget::JukeBoxSessionWidget(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::JukeBoxSessionWidget)
{
    ui->setupUi(this);
}

JukeBoxSessionWidget::~JukeBoxSessionWidget()
{
    delete ui;
}
