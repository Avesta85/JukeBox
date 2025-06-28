#include "qdebug.h"
#include "qthread.h"
#include <src/backend/network/ConnectionManager.h>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTcpServer>
#include <src/backend/network/filetransferworker.h>

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

void ConnectionManager::sendPlayCommand(const QHostAddress &targetAddress, quint16 targetPort)
{
    sendCommand(targetAddress,targetPort , NetworkCommand::Play,QVariant());
}

void ConnectionManager::sendPauseCommand(const QHostAddress& targetAddress, quint16 targetPort)
{

    sendCommand(targetAddress, targetPort, NetworkCommand::Pause, QVariant());
}

void ConnectionManager::sendSeekCommand(const QHostAddress& targetAddress, quint16 targetPort, qint64 position)
{

    sendCommand(targetAddress, targetPort, NetworkCommand::Seek, QVariant::fromValue(position));
}
void ConnectionManager::startListening(quint64 port)
{
    if(m_udpSocket->bind(QHostAddress::Any , port)){
        qDebug()<< "successfully start listening on port"<<port;
    }
    else{
        qDebug() << "Error: Could not bind to port" << port << m_udpSocket->errorString();
    }

    if (m_tcpserver->listen(QHostAddress::Any, port + 1)) {
        qDebug() << "TCP Server is now listening for file transfers on port" << port + 1;
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

    qDebug() << "New file transfer connection request received!";
}
