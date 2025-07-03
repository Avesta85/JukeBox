#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H
#include "qvideowidget.h"
#include <QStackedWidget>

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

signals:
    void videoFileSelected(const QString& filePath);

private:
    Ui::StageWidget *ui;

    MovieManagementWindow* m_movieManagementPage;

private slots:
    void onFilterChanged(int index);
    void onOpenButtonClicked();

};

#endif // STAGEWIDGET_H

