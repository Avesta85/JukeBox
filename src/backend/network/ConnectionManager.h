#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include <QObject>
#include <memory>
#include <QHostAddress>
#include <src/backend/network/NetworkProtocol.h>

class QUdpSocket;
class QTcpServer;

class ConnectionManager : public QObject
{
    Q_OBJECT

public:

    static ConnectionManager& getInstance();
    ~ConnectionManager();
    ConnectionManager(const ConnectionManager&)=delete;
    void operator=(const ConnectionManager&) = delete ;



public slots:

    void startListening(quint64 port = 8585);
    void sendCommand(const QHostAddress& targetAddress, quint16 targetPort, NetworkCommand command, const QVariant& payload);
    void sendFile(const QString& filePath, const QHostAddress& targetAddress, quint16 targetPort);


    void sendPlayCommand(const QHostAddress& targetAddress, quint16 targetPort);
    void sendPauseCommand(const QHostAddress& targetAddress, quint16 targetPort);
    void sendSeekCommand(const QHostAddress& targetAddress, quint16 targetPort, qint64 position);
signals :

    void commandReceived(NetworkCommand command, const QVariant& payload, const QHostAddress& sender);
    void fileTransferProgress(int percentage);
    void fileTransferFinished();
    void fileTransferError(const QString& errorString);
    void fileReceiveRequest(const QString& fileName, const QHostAddress& senderAddress, quint16 filePort);
private :
    ConnectionManager(QObject *parent = nullptr);
    static std::unique_ptr<ConnectionManager> s_instance;

    QUdpSocket* m_udpSocket;
    QTcpServer* m_tcpserver;

private slots :

    void onUdpDataReceived();
    void onNewFileConnection();
};




#endif // CONNECTIONMANAGER_H
