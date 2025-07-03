#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H
#include "qvideowidget.h"
#include <QStackedWidget>
#include "src/backend/core/Visualizer.h"

class playmusicwindow;
class MovieManagementWindow;

class QWidget;

namespace Ui {
class StageWidget;
}

class StageWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit StageWidget(QWidget *parent = nullptr);
    ~StageWidget();

    QVideoWidget* getVideoManagementWidget();

public slots:
    void showMovieManagementPage();
    void showCoverArtPage();
    void showVideoPage();
    void showVideoManagementPage();
    void showVisualizerPage();

signals:
    void videoFileSelected(const QString& filePath);

private:
    Ui::StageWidget *ui;

    MovieManagementWindow* m_movieManagementPage;
    AudioVisualizer* visualizer;

private slots:
    void onFilterChanged(int index);
    void onOpenButtonClicked();

    void on_comboBox_currentIndexChanged(int index);
};

#endif // STAGEWIDGET_H

