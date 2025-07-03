// audio_visualizer.cpp
#include "visualizer.h" // اصلاح شده از "visualizer.h"
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>
#include <QRandomGenerator> // برای افزودن حرکت تصادفی ظریف به نوارها
#include <QtMath>          // برای توابع ریاضی مثل qPow

// مقداردهی اولیه نمونه تکی
AudioVisualizer* AudioVisualizer::instance = nullptr;

AudioVisualizer::AudioVisualizer(QWidget *parent)
    : QWidget(parent),
    m_playbackState(QMediaPlayer::StoppedState),
    m_numBars(32), // می‌توانید تعداد نوارها را تنظیم کنید
    m_currentVolumeLevel(0.0),
    m_currentPattern(Pattern::Bars) // الگوی پیش‌فرض: نوارها
{
    // یک اندازه حداقل برای ویژوالایزر تنظیم کنید
    setMinimumSize(200, 100);
    // پس‌زمینه شفاف را فعال کنید (اگر می‌خواهید ویژوالایزر روی چیزی دیگر قرار گیرد).
    setAttribute(Qt::WA_TranslucentBackground);
    // خودمان پس‌زمینه را نقاشی خواهیم کرد
    setAutoFillBackground(false);

    initializeBarHeights();

    // تنظیم تایمر انیمیشن
    m_animationTimer.setInterval(50); // هر 50 میلی‌ثانیه به‌روزرسانی کنید (20 FPS)
    connect(&m_animationTimer, &QTimer::timeout, this, &AudioVisualizer::animateBars);

    // تایمر را ابتدا شروع کنید، توسط وضعیت پخش کنترل خواهد شد
    m_animationTimer.start();
}

void AudioVisualizer::initializeBarHeights()
{
    m_barHeights.resize(m_numBars);
    for (int i = 0; i < m_numBars; ++i) {
        m_barHeights[i] = 0.0; // همه نوارها با ارتفاع صفر شروع می‌شوند
    }
}

// پیاده‌سازی متد getInstance()
AudioVisualizer* AudioVisualizer::getInstance()
{
    if(!instance) {
        // توجه: در الگوی Singleton برای ویجت‌ها، معمولاً parent را null قرار می‌دهند
        // یا آن را به ویجت اصلی برنامه (مثلاً QMainWindow) متصل می‌کنند.
        // اگر Parent را null بگذارید، باید خودتان مدیریت حافظه (delete instance) را انجام دهید.
        // برای سادگی در این مثال، آن را null می‌گذاریم.
        instance = new AudioVisualizer(nullptr);
    }
    return instance;
}

void AudioVisualizer::setPlaybackState(QMediaPlayer::PlaybackState state)
{
    qDebug() << "Visualizer: Playback state changed to" << state;
    if (m_playbackState != state) {
        m_playbackState = state;
        if (m_playbackState == QMediaPlayer::PlayingState) {
            m_animationTimer.start(); // انیمیشن را هنگام پخش شروع کنید
        } else {
            // هنگام مکث یا توقف، نوارها به آرامی به صفر یا یک انیمیشن بیکار ظریف کاهش می‌یابند
            // تایمر را روشن نگه می‌داریم اما منطق انیمیشن را تغییر می‌دهیم
            if (m_barHeights[0] < 0.01) { // اگر خیلی کم است، حرکت تصادفی ظریفی اضافه کنید
                // این بخش برای حالت بیکار است، اما برای همگام‌سازی بهتر،
                // اجازه می‌دهیم نوارها در حالت مکث/توقف به طور کامل به صفر برگردند.
                // اگر می‌خواهید حرکت بیکار داشته باشید، این خط را فعال کنید:
                // m_barHeights[i] = (QRandomGenerator::global()->generateDouble() * 0.02);
            }
        }
        update(); // درخواست بازنقاشی
    }
}

void AudioVisualizer::updateAudioLevel(int volumePercent)
{
    // میزان صدا را به محدوده 0.0 - 1.0 نرمال کنید
    m_currentVolumeLevel = static_cast<double>(volumePercent) / 100.0;
}

void AudioVisualizer::setPattern(Pattern newPattern)
{
    if (m_currentPattern != newPattern) {
        m_currentPattern = newPattern;
        qDebug() << "Visualizer pattern changed to:" << static_cast<int>(newPattern);
        update(); // درخواست بازنقاشی برای نمایش الگوی جدید
    }
}

void AudioVisualizer::animateBars()
{
    // ضریب کاهش: سرعت افت نوارها
    const double decayFactor = 0.85; // بازگشت به مقدار اصلی شما
    // ضریب افزایش: میزان تأثیر صدا بر ارتفاع نوار
    const double boostFactor = 0.8; // بازگشت به مقدار اصلی شما

    for (int i = 0; i < m_numBars; ++i) {
        // اعمال کاهش
        m_barHeights[i] *= decayFactor;

        // اعمال سطح صدای جدید، با مقداری تصادفی بودن برای ظاهری طبیعی‌تر
        // این باعث می‌شود که نوارهای مختلف کمی متفاوت واکنش نشان دهند
        double randomOffset = (QRandomGenerator::global()->generateDouble() - 0.5) * 0.1; // بازگشت به مقدار اصلی شما
        double targetHeight = m_currentVolumeLevel * boostFactor; // بازگشت به محاسبه اصلی شما

        // اگر در حال پخش است، اطمینان حاصل کنید که نوارها به صدا واکنش نشان می‌دهند
        if (m_playbackState == QMediaPlayer::PlayingState) {
            // فقط در صورتی ارتفاع را افزایش دهید که میزان صدای جدید بالاتر باشد
            if (targetHeight + randomOffset > m_barHeights[i]) {
                m_barHeights[i] = targetHeight + randomOffset;
            }
        } else {
            // هنگام مکث/توقف، به یک سطح بیکار بسیار کوچک یا صفر کاهش یابید
            if (m_barHeights[i] < 0.01) { // اگر خیلی کم است، حرکت تصادفی ظریفی اضافه کنید
                m_barHeights[i] = (QRandomGenerator::global()->generateDouble() * 0.02); // بازگشت به مقدار اصلی شما
            }
        }

        // مقادیر را بین 0 و 1 محدود کنید
        m_barHeights[i] = qBound(0.0, m_barHeights[i], 1.0);
    }
    update(); // درخواست بازنقاشی پس از به‌روزرسانی ارتفاع نوارها
}

void AudioVisualizer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // پس‌زمینه را ترسیم کنید
    painter.fillRect(rect(), QColor(30, 30, 30, 180)); // پس‌زمینه تیره نیمه‌شفاف

    // بر اساس الگوی فعلی، تابع ترسیم مناسب را فراخوانی کنید
    switch (m_currentPattern) {
    case Pattern::Bars:
        drawBars(painter);
        break;
    case Pattern::Circles:
        drawCircles(painter);
        break;
    case Pattern::Waveform:
        drawWaveform(painter);
        break;
    default:
        drawBars(painter); // پیش‌فرض
        break;
    }
}

void AudioVisualizer::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update(); // درخواست بازنقاشی
}

// --- پیاده‌سازی توابع ترسیم برای الگوهای مختلف ---

void AudioVisualizer::drawBars(QPainter& painter)
{
    int barWidth = (width() - (m_numBars + 1) * 2) / m_numBars;
    if (barWidth <= 0) barWidth = 1;

    int spacing = 2;

    QLinearGradient barGradient(0, height(), 0, 0);
    barGradient.setColorAt(0.0, QColor(0, 150, 255));
    barGradient.setColorAt(0.5, QColor(0, 200, 255));
    barGradient.setColorAt(0.8, QColor(0, 255, 200));
    barGradient.setColorAt(1.0, QColor(255, 255, 0));

    painter.setBrush(barGradient);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < m_numBars; ++i) {
        int barHeight = static_cast<int>(m_barHeights[i] * height());
        if (barHeight < 1) barHeight = 1;

        int x = i * (barWidth + spacing) + spacing;
        int y = height() - barHeight;

        painter.drawRoundedRect(x, y, barWidth, barHeight, 3, 3);
    }
}

void AudioVisualizer::drawCircles(QPainter& painter)
{
    painter.setPen(Qt::NoPen); // دایره‌ها بدون حاشیه

    // گرادیان رنگی مشابه نوارها
    QLinearGradient circleGradient(0, height(), 0, 0);
    circleGradient.setColorAt(0.0, QColor(0, 150, 255));
    circleGradient.setColorAt(0.5, QColor(0, 200, 255));
    circleGradient.setColorAt(0.8, QColor(0, 255, 200));
    circleGradient.setColorAt(1.0, QColor(255, 255, 0));

    // استفاده مستقیم از گرادیان برای پر کردن دایره
    painter.setBrush(circleGradient); // <--- اصلاح اصلی برای نمایش دایره‌ها

    // برای هر نوار، یک دایره ترسیم کنید
    int circleSpacing = width() / m_numBars;
    int maxRadius = qMin(circleSpacing / 2 - 2, height() / 2 - 2); // حداکثر شعاع ممکن

    for (int i = 0; i < m_numBars; ++i) {
        // شعاع دایره بر اساس ارتفاع نوار
        int radius = static_cast<int>(m_barHeights[i] * maxRadius);
        if (radius < 1) radius = 1; // حداقل شعاع برای قابل مشاهده بودن

        // مرکز دایره
        int centerX = i * circleSpacing + circleSpacing / 2;
        int centerY = height() / 2; // در مرکز عمودی ویجت

        // تنظیم شفافیت (اختیاری، اگر می‌خواهید دایره‌های کوچک‌تر شفاف‌تر باشند)
        // این بخش را حذف کردم تا دایره‌ها همیشه با رنگ کامل گرادیان نمایش داده شوند.
        // اگر می‌خواهید شفافیت را برگردانید، می‌توانید آن را اینجا اضافه کنید.
        // int alpha = static_cast<int>(m_barHeights[i] * 255);
        // alpha = qBound(50, alpha, 255);
        // QColor brushColor = circleGradient.stops().first().second;
        // brushColor.setAlpha(alpha);
        // painter.setBrush(brushColor);

        painter.drawEllipse(QPoint(centerX, centerY), radius, radius);
    }
}

void AudioVisualizer::drawWaveform(QPainter& painter)
{
    painter.setBrush(Qt::NoBrush); // بدون پر کردن

    // گرادیان برای خط شکل موج
    QLinearGradient lineGradient(0, height(), 0, 0);
    lineGradient.setColorAt(0.0, QColor(0, 200, 255));
    lineGradient.setColorAt(1.0, QColor(255, 255, 0));
    painter.setPen(QPen(lineGradient, 3)); // خط با گرادیان و عرض 3 پیکسل

    QPolygonF waveformPolygon;
    double xStep = static_cast<double>(width()) / (m_numBars - 1);

    // افزودن نقاط به چندضلعی
    for (int i = 0; i < m_numBars; ++i) {
        double x = i * xStep;
        // ارتفاع از پایین به بالا
        double y = height() - (m_barHeights[i] * height());
        // کمی صاف کردن نقاط برای شکل موج نرم‌تر
        y = qBound(0.0, y, static_cast<double>(height())); // اطمینان از قرارگیری در محدوده ویجت
        waveformPolygon << QPointF(x, y);
    }

    // ترسیم خطوط بین نقاط
    painter.drawPolyline(waveformPolygon);

    // می‌توانید یک خط بیس‌لاین نیز اضافه کنید
    painter.setPen(QPen(QColor(100, 100, 100, 150), 1, Qt::DotLine));
    painter.drawLine(0, height() / 2, width(), height() / 2);
}
