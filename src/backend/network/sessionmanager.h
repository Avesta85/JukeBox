#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "src/backend/core/person.h"
#include "src/backend/network/NetworkProtocol.h"
#include <QObject>
#include <QObject>
#include <Qstring>
#include <QHostAddress>
#include <QTimer>
#include <QMap>



struct Participant {
    QString username;
    QHostAddress address;
    quint16 port;
    bool operator==(const Participant& other) const {
        return username == other.username;
    }
};

class QTcpSocket;
class QFile;

class SessionManager : public QObject
{
    Q_OBJECT
public:
    static SessionManager& getInstance();
    ~SessionManager();
    SessionManager(const SessionManager&) = delete;
    void operator=(const SessionManager&) = delete;
    bool getSessioonActive();


public slots:


    void startNewSession(const QString& hostUsername, const QHostAddress& hostAddress, quint16 hostPort);
    void joinSession(const QHostAddress& hostAddress,  quint16 hostPort, const QString& myUsername);
    void leaveSession();

    void onPlaySongRequested(const Song& song);
    void onChatMessageSendRequested(const QString& message);
    void onFileTransferAccepted(const QString& senderUsername, const QString& fileName);
private slots:

     void processNetworkCommand(NetworkCommand command, const QVariant& payload, const QHostAddress& sender, quint16 senderPort);
     void onCheckSongResponseTimeout();
signals:

    void participantListChanged(const QList<Person>& participants);
    void newChatMessageForUI(const QString& formattedMessage);
    void requestPermissionToReceiveFile(const QString& senderUsername, const QString& fileName);
    void showInfoMessage(const QString& message);


    // signals for PlayerManager


private :
    explicit SessionManager(QObject *parent = nullptr);


    static std::unique_ptr<SessionManager> s_instance;

    bool m_isSessionActive;
    bool m_isHost;
    QList<Participant> m_participants;
    QHostAddress m_hostAddress;
    Participant m_localUser;

    QMap<QString,QStringList>m_pendingSongRequest;

    QTimer* m_checkSongResponseTimer;


    QString findUsernameByAddress(const QHostAddress& address)const;
    void broadcastCommand(NetworkCommand command, const QVariant& payload, const QHostAddress* exclude = nullptr);
    void broadcastParticipantList();
    QList<Person> participantsAsPersonList() const;
};

#endif // SESSIONMANAGER_H
