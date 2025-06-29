#include "qdebug.h"
#include "qthread.h"
#include <src/backend/network/ConnectionManager.h>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTcpServer>
#include <QDir>
#include <src/backend/network/filetransferworker.h>
#include <src/backend/network/filereceiverworker.h>
#include <QStandardPaths>

std::unique_ptr<ConnectionManager> ConnectionManager::s_instance = nullptr;

ConnectionManager &ConnectionManager::getInstance()
{
    if(!s_instance)
    {
        s_instance.reset( new ConnectionManager());
    }
    return *s_instance;


}

void ConnectionManager::sendFile(const QString &filePath, const QHostAddress &targetAddress, quint16 targetPort)
{

    QThread* thread = new QThread(this);
    FileTransferWorker* worker = new FileTransferWorker(filePath,targetAddress,targetPort);

    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &FileTransferWorker::startTransfer);
    connect(worker, &FileTransferWorker::finished, thread, &QThread::quit);
    connect(worker, &FileTransferWorker::finished, worker, &FileTransferWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);


    connect(worker, &FileTransferWorker::progress, this, &ConnectionManager::fileTransferProgress);
    connect(worker, &FileTransferWorker::finished, this, &ConnectionManager::fileTransferFinished);
    connect(worker, &FileTransferWorker::error, this, &ConnectionManager::fileTransferError);

    thread->start();
}

void ConnectionManager::startListening()
{
    if(m_udpSocket->bind(QHostAddress::Any ,UDP_PORT)){
        qDebug()<< "successfully start listening on port"<<UDP_PORT;
    }
    else{
        qDebug() << "Error: Could not bind to port" << UDP_PORT << m_udpSocket->errorString();
    }

    if (m_tcpserver->listen(QHostAddress::Any, TCP_PORT)) {
        qDebug() << "TCP Server is now listening for file transfers on port" << TCP_PORT;
    } else {
        qDebug() << "Error: Could not start TCP server.";
    }
}

void ConnectionManager::sendCommand(const QHostAddress &targetAddress, quint16 targetPort, NetworkCommand command, const QVariant &payload)
{
    QByteArray datagram;
    QDataStream out(&datagram , QIODevice::WriteOnly);

    out << static_cast<qint32>(command);
    out << payload;

    m_udpSocket->writeDatagram(datagram,targetAddress,targetPort);

}

ConnectionManager::ConnectionManager(QObject *parent)
    :QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);

    connect(m_udpSocket,&QUdpSocket::readyRead,this,&ConnectionManager::onUdpDataReceived);

    m_tcpserver = new QTcpServer(this);
    connect(m_tcpserver,&QTcpServer::newConnection,this,&ConnectionManager::onNewFileConnection);

}

void ConnectionManager::onUdpDataReceived()
{
    while(m_udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        QDataStream in (datagram.data());


        qint32 commandInt;
        QVariant payload;


        in >> commandInt >> payload;

        NetworkCommand command = static_cast<NetworkCommand>(commandInt);
        QHostAddress senderAddress = datagram.senderAddress();

        emit commandReceived(command, payload , senderAddress);
    }
}

void ConnectionManager::onNewFileConnection()
{
    QTcpSocket* receivingSocket = m_tcpserver->nextPendingConnection();

    if(!receivingSocket)
    {
        qDebug() << "Error: could not get pending connection.";
        return;
    }
    qDebug() << "New file transfer connection request received!";

    QString savePath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);

    if (savePath.isEmpty()) {
        qDebug() << "Could not find standard music location, falling back to temp directory.";
        savePath = QDir::tempPath();
    }

    QDir dir(savePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QThread* thread = new QThread(this);
    FileReceiverWorker* worker = new FileReceiverWorker(receivingSocket, savePath);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &FileReceiverWorker::startReceiving);

    connect(worker, &FileReceiverWorker::finished, thread, &QThread::quit);
    connect(worker, &FileReceiverWorker::error, thread, &QThread::quit);
    connect(worker, &FileReceiverWorker::finished, worker, &FileReceiverWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    // connect(worker, &FileReceiverWorker::progress, this, &ConnectionManager::fileTransferProgress);

    thread->start();

    qDebug() << "FileReceiverWorker in a new thread started to handle the incoming file.";
}
