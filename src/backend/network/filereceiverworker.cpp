#include "filereceiverworker.h"
#include <QTcpSocket>
#include <QFile>
#include <QDataStream>
#include <qdebug.h>
#include <windows.h>



FileReceiverWorker::FileReceiverWorker(QTcpSocket* socket, const QString& savePath, QObject *parent)
    : QObject{parent},
    m_socket(socket),
    m_savePath(savePath),
    m_totalbytes(0),
    m_bytesReceived(0)
{

    m_socket->setParent(this);
}

FileReceiverWorker::~FileReceiverWorker()
{

    if (m_file.isOpen()) {
        m_file.close();
    }
}


void FileReceiverWorker::startReceiving()
{

    connect(m_socket, &QTcpSocket::readyRead, this, &FileReceiverWorker::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &FileReceiverWorker::onDisconnected);

    qDebug() << "FileReceiverWorker: Ready to receive data.";

    qDebug() << "📁 Target save path:" << m_savePath;
}

void FileReceiverWorker::onReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_15);

    if (m_totalbytes == 0) {
        if (m_socket->bytesAvailable() < (qint64)sizeof(qint64)) return;
        in >> m_totalbytes;

        if (m_socket->bytesAvailable() < (qint64)sizeof(qint32)) return;
        in >> m_fileName;

        QString filePath = m_savePath + "/" + m_fileName;
        qDebug() << "📥 Preparing to save file:" << m_fileName;
        qDebug() << "📁 Full path:" << filePath;
        qDebug() << "📦 Total size to receive:" << m_totalbytes << "bytes";

        m_file.setFileName(filePath);
        if (!m_file.open(QIODevice::WriteOnly)) {
            QString err = m_file.errorString();
            qDebug() << "❌ Failed to open file for writing:" << err;
            emit error("Could not open file: " + err);
            m_socket->close();
            return;
        }

        m_bytesReceived = 0;
    }

    QByteArray data = m_socket->readAll();
    qint64 written = m_file.write(data);

    if (written != data.size()) {
        qDebug() << "⚠️ Warning: Not all data written to file!";
    }

    m_bytesReceived += written;
    qDebug() << "📤 Bytes received so far:" << m_bytesReceived << "/" << m_totalbytes;

    if (m_totalbytes > 0) {
        int percentage = (m_bytesReceived * 100) / m_totalbytes;
        emit progress(percentage);
    }

    if (m_bytesReceived >= m_totalbytes) {
        m_file.flush();
        m_file.close();
        qDebug() << "✅ File received successfully. Saved at:" << m_file.fileName();
        emit finished(m_file.fileName());

        m_socket->close();  // Clean close
    }
}


void FileReceiverWorker::onDisconnected()
{
    qDebug() << "📴 FileReceiverWorker: Host disconnected.";

    if (m_socket->bytesAvailable() > 0) {
        QByteArray remaining = m_socket->readAll();
        qint64 written = m_file.write(remaining);
        m_bytesReceived += written;
        qDebug() << "📥 Wrote remaining" << written << "bytes from buffer after disconnect";
    }

    qDebug() << m_bytesReceived;
    qDebug() << m_totalbytes;
    if (m_bytesReceived + 1000 >= m_totalbytes) {
        if (m_file.isOpen()) {
            m_file.flush();
            m_file.close();
        }
        qDebug() << "✅ File received successfully after disconnect. Saved at:" << m_file.fileName();
        emit finished(m_file.fileName());
    } else {
        qDebug() << "❌ Incomplete file — deleting it.";
        if (m_file.isOpen()) {
            m_file.close();
            m_file.remove();
        }
        emit error("Connection closed prematurely. File deleted.");
    }
}


