#include "filereceiverworker.h"
#include <QTcpSocket>
#include <QFile>
#include <QDataStream>
#include <qdebug.h>


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
}

void FileReceiverWorker::onReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_15);


    if (m_totalbytes == 0) {

        if (m_socket->bytesAvailable() < (qint64)sizeof(qint64)) {
            return;
        }

        in >> m_totalbytes;


        if (m_socket->bytesAvailable() < (qint64)sizeof(qint32)) {
            return;
        }

        in >> m_fileName;

        qDebug() << "FileReceiverWorker: Receiving file" << m_fileName << "with size" << m_totalbytes;


        QString filePath = m_savePath + "/" + m_fileName;
        m_file.setFileName(filePath);
        if (!m_file.open(QIODevice::WriteOnly)) {
            emit error("Could not open file for writing: " + m_file.errorString());
            m_socket->close();
            return;
        }
    }

    QByteArray data = m_socket->readAll();
    m_file.write(data);
    m_bytesReceived += data.size();

    if (m_totalbytes > 0) {
        int percentage = (m_bytesReceived * 100) / m_totalbytes;
        emit progress(percentage);
    }

    if (m_bytesReceived >= m_totalbytes) {
        qDebug() << "FileReceiverWorker: File received successfully. Saved at" << m_file.fileName();
        m_file.close();
        emit finished(m_file.fileName());

    }
}

void FileReceiverWorker::onDisconnected()
{
    qDebug() << "FileReceiverWorker: Host disconnected.";

    if (m_bytesReceived < m_totalbytes) {
        emit error("Connection was closed prematurely.");
        if (m_file.isOpen()) {
            m_file.close();
            m_file.remove();
        }
    }

}
