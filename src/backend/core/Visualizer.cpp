#include "Visualizer.h"
#include <QPainter>
#include <QDebug>
#include <QPainterPath>
#include <QRandomGenerator>
#include <cmath>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

VisualizerWidget::VisualizerWidget(QWidget *parent)
    : QWidget(parent), timer(new QTimer(this)) {
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&VisualizerWidget::update));
    timer->start(30);
}

VisualizerWidget::~VisualizerWidget() {
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}

void VisualizerWidget::loadAndPlay(const QString &filePath) {
    if (ma_decoder_init_file(filePath.toUtf8().constData(), nullptr, &decoder) != MA_SUCCESS) {
        qWarning() << "Failed to load audio";
        return;
    }

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = decoder.outputFormat;
    config.playback.channels = decoder.outputChannels;
    config.sampleRate        = decoder.outputSampleRate;
    config.dataCallback      = dataCallback;
    config.pUserData         = this;

    if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS) {
        qWarning() << "Failed to init device";
        return;
    }

    ma_device_start(&device);
}

void VisualizerWidget::dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount) {
    auto *self = reinterpret_cast<VisualizerWidget*>(device->pUserData);
    ma_decoder_read_pcm_frames(&self->decoder, output, frameCount, nullptr);
    const float *data = static_cast<float *>(output);
    self->processAudio(data, frameCount);
}

void VisualizerWidget::processAudio(const float *data, ma_uint32 count) {
    QMutexLocker locker(&mutex);
    samples.clear();
    for (ma_uint32 i = 0; i < count; i += 1) {
        samples.append(data[i]);
    }
}

void VisualizerWidget::setVisualizerMode(VisualizerMode newMode) {
    mode = newMode;
    if (mode == VisualizerMode::Random) {
        int r = QRandomGenerator::global()->bounded(2); // 0 یا 1
        mode = (r == 0) ? VisualizerMode::Bars : VisualizerMode::Waveform;
    }
    update();
}

void VisualizerWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), Qt::black);

    QMutexLocker locker(&mutex);
    if (samples.isEmpty()) return;

    switch (mode) {
    case VisualizerMode::Bars:
        drawBars(p);
        break;
    case VisualizerMode::Waveform:
        drawWaveform(p);
        break;
    default:
        break;
    }
}

void VisualizerWidget::drawBars(QPainter &p) {
    int w = width();
    int h = height();
    const int barCount = 100;
    QVector<float> bars;
    int chunkSize = std::max(1, static_cast<int>(samples.size()) / barCount);

    for (int i = 0; i < barCount; ++i) {
        float sum = 0;
        for (int j = 0; j < chunkSize; ++j) {
            int idx = i * chunkSize + j;
            if (idx < samples.size()) sum += std::abs(samples[idx]);
        }
        bars.append(sum / chunkSize);
    }

    float barWidth = float(w) / barCount;

    for (int i = 0; i < bars.size(); ++i) {
        float value = qBound(0.0f, bars[i], 1.0f);
        float barHeight = h * value * 0.9f;

        QColor color = QColor::fromHsvF(0.35 + 0.6 * value, 1.0, 1.0);
        p.setBrush(color);
        p.setPen(Qt::NoPen);

        float x = i * barWidth;
        float y = h - barHeight;

        p.drawRect(QRectF(x, y, barWidth, barHeight));
    }


}

void VisualizerWidget::drawWaveform(QPainter &p) {
    int w = width();
    int h = height();
    int count = samples.size();
    if (count < 2) return;

    float step = float(w) / (count - 1);
    QPainterPath path;
    path.moveTo(0, h / 2 - samples[0] * h / 2);

    for (int i = 1; i < count; ++i) {
        float x = i * step;
        float y = h / 2 - samples[i] * h / 2;
        path.lineTo(x, y);
    }

    QPen pen(QColor(100, 255, 200), 2);
    p.setPen(pen);
    p.drawPath(path);


}

