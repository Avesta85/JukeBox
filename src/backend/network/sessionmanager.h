#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "src/backend/core/person.h"
#include "src/backend/network/NetworkProtocol.h"
#include <QObject>
#include <QObject>
#include <Qstring>
#include <QHostAddress>




struct Participant {
    QString username;
    QHostAddress address;

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



public slots:


    void startNewSession(const QString& hostUsername);
    void joinSession(const QHostAddress& hostAddress,  quint16 hostPort, const QString& myUsername);
    void leaveSession();

    void sendChatMessage(const QString& message);
    void requestToPlaySong(const Song& song);

    void processNetworkCommand(NetworkCommand command, const QVariant& payload, const QHostAddress& sender);


    void onPlaySongRequested(const Song& song);
    void onChatMessageSendRequested(const QString& message);
signals:

    void participantListChanged(const QList<Person>& participants);
    void newChatMessageForUI(const QString& formattedMessage);
    void newSongForPlayback(const Song& song);
    void showInformation(const QString& message);
    void requestFileFromPeer(const QString& fileName, const QHostAddress& address);

private :
    explicit SessionManager(QObject *parent = nullptr);


    static std::unique_ptr<SessionManager> s_instance;

    bool m_isSessionActive;
    bool m_isHost;
    QList<Participant> m_participants;
    QHostAddress m_hostAddress;
    Participant m_localUser;
    quint16 m_hostPort;


    QString findUsernameByAddress(const QHostAddress& address)const;
};

#endif // SESSIONMANAGER_H
