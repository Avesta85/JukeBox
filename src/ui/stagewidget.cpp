#include "stagewidget.h"
#include "ui_stagewidget.h"

#include "playmusicwindow.h"
#include "moviemanagementwindow.h"


StageWidget::StageWidget(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::StageWidget)
{
    ui->setupUi(this);


    m_musicManagementPage = new playmusicwindow(this);
    m_movieManagementPage = new MovieManagementWindow(this);

    this->addWidget(m_musicManagementPage);
    this->addWidget(m_movieManagementPage);

    showCoverArtPage();
}

StageWidget::~StageWidget()
{
    delete ui;

}


void StageWidget::showMusicManagementPage()
{
    this->setCurrentWidget(m_musicManagementPage);
}

void StageWidget::showMovieManagementPage()
{
    this->setCurrentWidget(m_movieManagementPage);
}

void StageWidget::showCoverArtPage()
{

    this->setCurrentWidget(ui->page_cover_art);
}

void StageWidget::showVideoPage()
{

    this->setCurrentWidget(ui->page_video);
}
