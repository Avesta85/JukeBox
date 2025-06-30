#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include "src/backend/network/ConnectionManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("🎧 Network Test Tool");

    auto *layout = new QVBoxLayout(&window);
    auto *btnStartListening = new QPushButton("🔊 Start Listening");
    auto *btnSendCommand = new QPushButton("📨 Send Test Message");
    auto *btnSendFile = new QPushButton("📁 Send File to Self");
    auto *inputMessage = new QLineEdit;
    inputMessage->setPlaceholderText("Type your test message here...");

    layout->addWidget(btnStartListening);
    layout->addWidget(inputMessage);
    layout->addWidget(btnSendCommand);
    layout->addWidget(btnSendFile);

    // When a command is received
    QObject::connect(&ConnectionManager::getInstance(), &ConnectionManager::commandReceived,
                     [](NetworkCommand cmd, const QVariant& payload, const QHostAddress& sender, quint16){
                         qDebug() << "✅ Received Command:";
                         qDebug() << "   Type:" << static_cast<int>(cmd);
                         qDebug() << "   Payload:" << payload;
                         qDebug() << "   Sender:" << sender.toString();
                     });

    // When file transfer is completed
    QObject::connect(&ConnectionManager::getInstance(), &ConnectionManager::fileTransferFinished,
                     [](const QString& path){
                         qDebug() << "✅ File transfer completed. Saved at:" << path;
                     });

    // When file transfer fails
    QObject::connect(&ConnectionManager::getInstance(), &ConnectionManager::fileTransferError,
                     [](const QString& err){
                         qDebug() << "❌ File transfer error:" << err;
                     });

    // Start listening
    QObject::connect(btnStartListening, &QPushButton::clicked, [=]() {
        ConnectionManager::getInstance().startListening();
    });

    // Send test UDP command
    QObject::connect(btnSendCommand, &QPushButton::clicked, [=]() {
        QString msg = inputMessage->text().trimmed();
        if (msg.isEmpty()) msg = "Hello! This is a test message.";

        ConnectionManager::getInstance().sendCommand(
            QHostAddress("127.0.0.1"),
            UDP_PORT,
            NetworkCommand::ChatMessage,
            msg
            );
    });

    // Send test file to self
    QObject::connect(btnSendFile, &QPushButton::clicked, [=]() {
        QString path = QFileDialog::getOpenFileName(nullptr, "Select file to send", "", "Audio Files (*.mp3 *.wav)");
        qDebug() << path;
        if (!path.isEmpty()) {
            ConnectionManager::getInstance().sendFile(
                path,
                QHostAddress("127.0.0.1"),
                TCP_PORT
                );
        }
    });

    window.resize(420, 220);
    window.show();
    return app.exec();
}
