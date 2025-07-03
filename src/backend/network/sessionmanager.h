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
#include <QString>
#include <QtGlobal>
#include <QList>
#include <QVariant>
#include <QMetaType>// Dummy struct for linter

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
    bool ISHost();

    void broadcastPlayCommand(qint64 position);
    void broadcastPauseCommand();
    void broadcastSeekCommand(qint64 position);
public slots:

    void RequestsyncSong(const Song& song);

    void startNewSession(const QString& hostUsername, const QHostAddress& hostAddress, quint16 hostPort);
    void joinSession(const QHostAddress& hostAddress,  const QString& myUsername);
    void leaveSession();

    void onPlaySongRequested(const Song& song);
    void onChatMessageSendRequested(const QString& message);
    void sendChatMessageToHost(const QString& message);
    void onFileTransferAccepted(const QString& senderUsername, const QString& fileName);
    void kickUser(const QString& username);
    void kickUsers(const QList<Person>& users);
    void sendLeaveRequestToHost();


private slots:

     void processNetworkCommand(NetworkCommand command, const QVariant& payload, const QHostAddress& sender, quint16 senderPort);
     void onCheckSongResponseTimeout();
signals:

    void songsynced();
    void joinRequestaccept();
    void joinRequestReject();
    void setCurrentSong(Song name);
    void syncStatusChanged(const QString& status);
    void syncError(const QString& error);

    void participantListChanged(const QList<Person>& participants);
    void newChatMessageForUI(const QString& formattedMessage);
    void requestPermissionToReceiveFile(const QString& senderUsername, const QString& fileName);
    void showInfoMessage(const QString& message);
    void kickedFromSession(const QString& reason);
    void systemMessage(const QString& message);




    void remotePlayRequested(qint64 position);
    void remotePauseRequested();
    void remoteSeekRequested(qint64 position);

private :
    explicit SessionManager(QObject *parent = nullptr);


    static std::unique_ptr<SessionManager> s_instance;

    bool m_isSessionActive;
    bool m_isHost;
    QList<Participant> m_participants;
    QHostAddress m_hostAddress;
    QString m_hostUsername;
    Participant m_localUser;

    QMap<QString,QStringList>m_pendingSongRequest;

    QTimer* m_checkSongResponseTimer;


    QString findUsernameByAddress(const QHostAddress& address)const;
    void broadcastCommand(NetworkCommand command, const QVariant& payload, const QHostAddress* exclude = nullptr);
    void broadcastParticipantList();
    QList<Person> participantsAsPersonList() const;


};

#endif // SESSIONMANAGER_H

