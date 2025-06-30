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

    QWidget* musicPageContainer = new QWidget(this);

    QVBoxLayout* musicLayout = new QVBoxLayout(musicPageContainer);
    musicLayout->setContentsMargins(0, 0, 0, 0);

    musicLayout->addWidget(m_musicManagementPage);

    this->addWidget(musicPageContainer);

    m_musicPageContainer = musicPageContainer;
    m_movieManagementPage = new MovieManagementWindow(this);
    QWidget* moviePageContainer = new QWidget(this);
    QVBoxLayout* movieLayout = new QVBoxLayout(moviePageContainer);
    movieLayout->setContentsMargins(0, 0, 0, 0);
    movieLayout->addWidget(m_movieManagementPage);
    this->addWidget(moviePageContainer);
    m_moviePageContainer = moviePageContainer;


    showCoverArtPage();
}

StageWidget::~StageWidget()
{
    delete ui;

}

playmusicwindow *StageWidget::getMusicManagementPage()
{
    return m_musicManagementPage;
}


void StageWidget::showMusicManagementPage()
{
    this->setCurrentWidget(m_musicPageContainer);
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
