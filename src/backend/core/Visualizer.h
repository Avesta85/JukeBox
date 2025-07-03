#ifndef AUDIO_VISUALIZER_H
#define AUDIO_VISUALIZER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QTimer>
#include <QVector>
#include <QPainter>

class AudioVisualizer : public QWidget
{
    Q_OBJECT

public:
    enum class Pattern {
        Bars,
        Circles,
        Waveform
    };
    Q_ENUM(Pattern)

    explicit AudioVisualizer(QWidget *parent = nullptr);
    static AudioVisualizer* getInstance();
    void setPlaybackState(QMediaPlayer::PlaybackState state);
    void updateAudioLevel(int volumePercent);

public slots:
    void setPattern(Pattern newPattern);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    static AudioVisualizer* instance;

    QMediaPlayer::PlaybackState m_playbackState;
    QTimer m_animationTimer;
    QVector<double> m_barHeights;
    int m_numBars;
    double m_currentVolumeLevel;
    Pattern m_currentPattern;

    void initializeBarHeights();
    void animateBars();

    void drawBars(QPainter& painter);
    void drawCircles(QPainter& painter);
    void drawWaveform(QPainter& painter);
};

#endif // AUDIO_VISUALIZER_H
