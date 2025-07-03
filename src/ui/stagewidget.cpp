#include "stagewidget.h"
#include "ui_stagewidget.h"

#include "playmusicwindow.h"
#include "moviemanagementwindow.h"

#include <QFileDialog>
#include <QGraphicsColorizeEffect>
#include <QSizePolicy>
#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QVideoWidget>
#include <QGraphicsProxyWidget>

StageWidget::StageWidget(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::StageWidget)
{
    ui->setupUi(this);

    m_movieManagementPage = new MovieManagementWindow(this);
    QWidget* moviePageContainer = new QWidget(this);
    QVBoxLayout* movieLayout = new QVBoxLayout(moviePageContainer);
    movieLayout->setContentsMargins(0, 0, 0, 0);
    movieLayout->addWidget(m_movieManagementPage);
    this->addWidget(moviePageContainer);
    moviePageContainer = moviePageContainer;

    // Connect filter button in video management page
    // connect(ui->button_filter, &QPushButton::clicked, this, &StageWidget::onFilterButtonClicked);
    connect(ui->comboBox_filter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StageWidget::onFilterChanged);
    connect(ui->button_open, &QPushButton::clicked, this, &StageWidget::onOpenButtonClicked);

    // In constructor, after setupUi, ensure videoWidget_main fits layout
    ui->videoWidget_main->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto* scene = new QGraphicsScene(this);
    ui->graphicsView_visualizer->setScene(scene);

    visualizer = AudioVisualizer::getInstance();
    auto* proxy = scene->addWidget(visualizer);
    proxy->setPos(100, 100);
    proxy->setFlag(QGraphicsItem::ItemIsMovable);

    showCoverArtPage();
}

StageWidget::~StageWidget()
{
    delete ui;

}

QVideoWidget *StageWidget::getVideoManagementWidget()
{
    return ui->videoWidget_main;
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

void StageWidget::showVideoManagementPage()
{
    this->setCurrentWidget(ui->page_video_management);
}

void StageWidget::showVisualizerPage()
{
    this->setCurrentWidget(ui->page_visualizer);
}

void StageWidget::onFilterChanged(int index) {
    if (!ui->videoWidget_main) return;
    if (index == 1) { // Grayscale
        QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect(ui->videoWidget_main);
        effect->setColor(Qt::gray);
        effect->setStrength(1.0);
        ui->videoWidget_main->setGraphicsEffect(effect);
    } else {
        ui->videoWidget_main->setGraphicsEffect(nullptr);
    }
}

void StageWidget::onOpenButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select a Video"), QDir::homePath(), tr("Video Files (*.mp4 *.avi *.mkv *.mov)"));
    if (!filePath.isEmpty()) {
        emit videoFileSelected(filePath);
    }
}

void StageWidget::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0)
        visualizer->setPattern(AudioVisualizer::Pattern::Bars);

    else if(index == 1)
        visualizer->setPattern(AudioVisualizer::Pattern::Waveform);
}

