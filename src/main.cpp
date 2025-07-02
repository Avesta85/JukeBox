#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include "src/backend/core/Visualizer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // ساخت پنجره اصلی
    QWidget window;
    window.setWindowTitle("Multi-Pattern Audio Visualizer 🎧");

    // اجزای UI
    auto *visualizer = new VisualizerWidget;
    auto *btnLoad = new QPushButton("انتخاب فایل MP3/WAV");
    auto *comboMode = new QComboBox;
    comboMode->addItems({"Bars", "Waveform", "Random"});

    // چیدمان
    auto *layout = new QVBoxLayout(&window);
    layout->addWidget(visualizer, 1); // ویژوالایزر
    layout->addWidget(comboMode);     // انتخاب حالت
    layout->addWidget(btnLoad);       // دکمه بارگذاری صدا

    // انتخاب حالت ویژوالایزر
    QObject::connect(comboMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&](int index) {
                         visualizer->setVisualizerMode(static_cast<VisualizerMode>(index));
                     });

    // بارگذاری فایل صدا
    QObject::connect(btnLoad, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(nullptr, "انتخاب فایل صوتی", "", "Audio Files (*.mp3 *.wav)");
        if (!path.isEmpty())
            visualizer->loadAndPlay(path);
    });

    window.resize(900, 500);
    window.show();

    return app.exec();
}
