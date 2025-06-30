#include "filetransferworker.h"
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <qdebug.h>

FileTransferWorker::FileTransferWorker(const QString &filePath, const QHostAddress &address, quint16 port, QObject *parent)
    : QObject{parent},
    m_filePath(filePath),
    m_targetAddress(address),
    m_targetPort(port),
    m_socket(nullptr),
    m_totalBytes(0),
    m_bytesWritten(0)

{
    ;
}

void FileTransferWorker::startTransfer()
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &FileTransferWorker::onConnected);
    connect(m_socket, &QTcpSocket::bytesWritten, this, &FileTransferWorker::onByteWritten);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &FileTransferWorker::onSocketError);

    qDebug() << "FileTransferWorker: Connecting to" << m_targetAddress.toString() << ":" << m_targetPort;
    m_socket->connectToHost(m_targetAddress, m_targetPort);
}

void FileTransferWorker::onConnected()
{
    qDebug() << "FileTransferWorker: Connected to host. Starting file transfer.";

    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error("Could not open file for reading: " + file.errorString());
        m_socket->close();
        return;
    }

    m_totalBytes = file.size();


    QByteArray header;
    QDataStream out(&header, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);


    out << (qint64)0;
    out << QFileInfo(m_filePath).fileName();

    out.device()->seek(0);
    out << (qint64)(header.size() + m_totalBytes - sizeof(qint64));

    // ابتدا هدر را ارسال می‌کنیم
    m_socket->write(header);

    qint64 bytesRemaining = m_totalBytes;
    while (bytesRemaining > 0) {
        QByteArray fileData = file.read(65536);
        m_socket->write(fileData);
        bytesRemaining -= fileData.size();
    }

    file.close();
    qDebug() << "FileTransferWorker: All data has been written to the socket.";
}

void FileTransferWorker::onByteWritten(qint64 bytes)
{
    m_bytesWritten += bytes;
    QFileInfo fp;

    if (m_totalBytes > 0) {
        int percentage = (m_bytesWritten * 100) / (m_totalBytes + m_socket->bytesToWrite() + 100); // +100 to avoid division by zero and include header
        emit progress(fp.fileName(), percentage);
    }

    if (m_bytesWritten >= m_totalBytes) {
        if (m_socket->bytesToWrite() == 0) {
            qDebug() << "FileTransferWorker: Transfer finished.";
            m_socket->disconnectFromHost();
            emit finished(fp.fileName());
        }
    }
}

void FileTransferWorker::onSocketError()
{
    emit error(m_socket->errorString());
    m_socket->close();
}
