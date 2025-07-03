// audio_visualizer.h
#ifndef AUDIO_VISUALIZER_H
#define AUDIO_VISUALIZER_H

#include <QWidget>
#include <QMediaPlayer> // برای استفاده از QMediaPlayer::PlaybackState
#include <QTimer>     // برای انیمیشن و به‌روزرسانی‌های منظم
#include <QVector>    // برای نگهداری ارتفاع نوارهای ویژوالایزر
#include <QPainter>   // برای عملیات ترسیم

/**
 * @brief کلاس AudioVisualizer یک نمایش بصری ساده از پخش صدا را ارائه می‌دهد.
 * این ویژوالایزر نوارها را بر اساس شدت باندهای فرکانسی مختلف ترسیم می‌کند.
 * همچنین به وضعیت پخش (مانند پخش، مکث یا توقف) واکنش نشان می‌دهد.
 */
class AudioVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief الگوهای بصری مختلف برای ویژوالایزر.
     */
    enum class Pattern {
        Bars,       /**< نوارها (پیش‌فرض). */
        Circles,    /**< دایره‌ها. */
        Waveform    /**< شکل موج ساده. */
    };
    Q_ENUM(Pattern) // برای استفاده از enum در QML یا متادیتا سیستم Qt

    /**
     * @brief سازنده کلاس AudioVisualizer.
     * @param parent ویجت والد این ویژوالایزر.
     */
    explicit AudioVisualizer(QWidget *parent = nullptr);

    /**
     * @brief نمونه تکی (Singleton) AudioVisualizer را برمی‌گرداند.
     * @return اشاره‌گر به نمونه AudioVisualizer.
     */
    static AudioVisualizer* getInstance();

    /**
     * @brief وضعیت پخش فعلی پلیر صوتی را تنظیم می‌کند.
     * این متد برای همگام‌سازی انیمیشن ویژوالایزر با وضعیت پخش موسیقی استفاده می‌شود.
     * @param state وضعیت پخش جدید (مثلاً QMediaPlayer::PlayingState, QMediaPlayer::PausedState).
     */
    void setPlaybackState(QMediaPlayer::PlaybackState state);

    /**
     * @brief سطح صدای فعلی را برای ویژوالایزر به‌روزرسانی می‌کند.
     * این متد باید به طور مکرر با استفاده از سیگنال volumeChanged از PlayerManager فراخوانی شود.
     * @param volumePercent میزان صدا به صورت درصد (0 تا 100).
     */
    void updateAudioLevel(int volumePercent);

public slots:
    /**
     * @brief الگوی بصری ویژوالایزر را تغییر می‌دهد.
     * @param newPattern الگوی جدید برای نمایش.
     */
    void setPattern(Pattern newPattern);

protected:
    /**
     * @brief رویداد paint را بازنویسی می‌کند تا ویژوالایزر را ترسیم کند.
     * @param event رویداد paint.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief رویداد resize را بازنویسی می‌کند تا هنگام تغییر اندازه ویجت،
     * طرح‌بندی و اندازه نوارهای ویژوالایزر به‌روزرسانی شود.
     * @param event رویداد resize.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    static AudioVisualizer* instance;            /**< نمونه تکی AudioVisualizer. */

    QMediaPlayer::PlaybackState m_playbackState; /**< وضعیت پخش فعلی (پخش، مکث، توقف). */
    QTimer m_animationTimer;                     /**< تایمر برای فعال کردن فریم‌های انیمیشن. */
    QVector<double> m_barHeights;                /**< ارتفاعات فعلی نوارهای ویژوالایزر (نرمال‌شده بین 0.0 تا 1.0). */
    int m_numBars;                               /**< تعداد نوارهایی که در ویژوالایزر نمایش داده می‌شوند. */
    double m_currentVolumeLevel;                 /**< سطح صدای فعلی (نرمال‌شده بین 0.0 تا 1.0). */
    Pattern m_currentPattern;                    /**< الگوی بصری فعلی. */

    /**
     * @brief ارتفاع اولیه نوارهای ویژوالایزر را تنظیم می‌کند (معمولاً به صفر).
     */
    void initializeBarHeights();

    /**
     * @brief این اسلات توسط m_animationTimer فراخوانی می‌شود تا ارتفاع نوارها را برای انیمیشن به‌روزرسانی کند.
     * شامل منطق کاهش ارتفاع و واکنش به سطح صدا است.
     */
    void animateBars(); // نام تابع همان animateBars باقی ماند.

    // --- توابع ترسیم برای الگوهای مختلف ---
    void drawBars(QPainter& painter);
    void drawCircles(QPainter& painter);
    void drawWaveform(QPainter& painter);
};

#endif // AUDIO_VISUALIZER_H
