#ifndef VISUALIZERWIDGET_H
#define VISUALIZERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMutex>
#include <QVector>
#include <QString>
#include "miniaudio.h"

enum class VisualizerMode {
    Bars,
    Waveform,
    Random
};

class VisualizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit VisualizerWidget(QWidget *parent = nullptr);
    ~VisualizerWidget();

    void loadAndPlay(const QString &filePath);
    void setVisualizerMode(VisualizerMode newMode);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    static void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);
    void processAudio(const float* data, ma_uint32 count);

    void drawBars(QPainter &p);
    void drawWaveform(QPainter &p);

    ma_decoder decoder;
    ma_device device;
    QVector<float> samples;
    QMutex mutex;
    QTimer *timer;

    VisualizerMode mode = VisualizerMode::Bars;
};

#endif // VISUALIZERWIDGET_H

