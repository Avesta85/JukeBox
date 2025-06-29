#include "sessionmanager.h"

#include "ConnectionManager.h"

std::unique_ptr<SessionManager> SessionManager::s_instance = nullptr;

SessionManager &SessionManager::getInstance()
{
    if(!s_instance)
    {
        s_instance.reset(new SessionManager);
    }
    return *s_instance;
}

SessionManager::~SessionManager() {}

void SessionManager::startNewSession(const QString &hostUsername, const QHostAddress &hostAddress, quint16 hostPort)
{
    leaveSession();
    m_isSessionActive = true;
    m_isHost = true;
    m_localUser = {hostUsername,hostAddress,hostPort};
    m_participants.append(m_localUser);


    ConnectionManager::getInstance().startListening();
    emit participantListChanged(participantsAsPersonList());
    emit showInfoMessage("New session created. Waiting for friends to join...");
}

void SessionManager::joinSession(const QHostAddress &hostAddress, quint16 hostPort, const QString &myUsername)
{
    leaveSession();
    m_isSessionActive = true;
    m_isHost = false;
    m_localUser = {myUsername,QHostAddress(),0};

    QVariantMap payload;

    payload["username"] = myUsername;
    ConnectionManager::getInstance().sendCommand(hostAddress,UDP_PORT,NetworkCommand::SessionJoinRequest,payload);
    emit showInfoMessage("Join request sent. Waiting for host to accept...");
}

void SessionManager::leaveSession()
{
    if(m_isSessionActive)
    {
        broadcastCommand(NetworkCommand::SessionLeave, m_localUser.username);
    }
        m_isSessionActive = false;
        m_isHost =false;
        m_participants.clear();

    emit participantListChanged({});
}

void SessionManager::onPlaySongRequested(const Song &song)
{
    if(!m_isSessionActive || !m_isHost) return;

    qDebug() << "Host requested to play song:" << song.getName();
    // TODO: پیاده‌سازی منطق چک کردن وجود آهنگ در کلاینت‌ها
    // برای سادگی فعلا فرض می‌کنیم همه آهنگ را دارند
    // emit ui_play();
     broadcastCommand(NetworkCommand::CheckSongRequest, song.getName());

}

void SessionManager::onChatMessageSendRequested(const QString &message)
{
    if(!m_isSessionActive || message.isEmpty()) return ;
    emit newChatMessageForUI("Me: "+message);
    broadcastCommand(NetworkCommand::ChatMessage,message);
}

void SessionManager::processNetworkCommand(NetworkCommand command, const QVariant &payload, const QHostAddress &sender,quint16 senderPort)
{
    if (!m_isSessionActive) return;

    if(m_isHost)
    {
        if(command == NetworkCommand::SessionJoinRequest)
        {
            QString newUsername= payload.toMap().value("username").toString();

            // TODO: باید از میزبان در UI سوال پرسیده شود که آیا این کاربر را می‌پذیرد یا نه
            // برای سادگی، فعلاً همه را به صورت خودکار می‌پذیریم
            Participant newParticipant = {newUsername, sender, senderPort};
            m_participants.append(newParticipant);
            broadcastParticipantList();
            emit showInfoMessage(QString("%1 has joined the session.").arg(newUsername));
        }
    }
    else
    {
        if (command == NetworkCommand::ParticipantListUpdate) {
            m_participants.clear();
            QVariantList list = payload.toList();
            for(const QVariant& item : list) {
                QVariantMap map = item.toMap();
                Participant p = {map["username"].toString(), QHostAddress(map["address"].toString()), static_cast<quint16>(map["port"].toUInt())};
                m_participants.append(p);
            }
            emit participantListChanged(participantsAsPersonList());
        }
    }
    if (command == NetworkCommand::ChatMessage) {
        QString message = payload.toString();
        QString senderUsername = findUsernameByAddress(sender);
        emit newChatMessageForUI(QString("%1: %2").arg(senderUsername, message));
    }
}

SessionManager::SessionManager(QObject *parent)
    : QObject{parent},
    m_isSessionActive(false),
    m_isHost(false)
{
    connect(&ConnectionManager::getInstance(), &ConnectionManager::commandReceived,
            this, &SessionManager::processNetworkCommand);
}

QString SessionManager::findUsernameByAddress(const QHostAddress &address) const
{

    for(const Participant& p:m_participants)
    {
        if(p.address == address)
            return p.username;
    }
    return "UnKnown User";
}

void SessionManager::broadcastCommand(NetworkCommand command, const QVariant &payload, const QHostAddress *exclude)
{
    for(const Participant& p : m_participants)
    {
        if(p.username !=m_localUser.username && (!exclude || p.address !=*exclude))
        {
            ConnectionManager::getInstance().sendCommand(p.address,UDP_PORT,command,payload);
        }
    }
}

void SessionManager::broadcastParticipantList()
{
    if (!m_isHost) return;
    QVariantList participantList;
    for(const Participant& p : m_participants) {
        QVariantMap map;
        map["username"] = p.username;
        map["address"] = p.address.toString();
        map["port"] = p.port;
        participantList.append(map);
    }
    broadcastCommand(NetworkCommand::ParticipantListUpdate, participantList);
}

QList<Person> SessionManager::participantsAsPersonList() const {
    QList<Person> personList;
    for(const auto& p : m_participants) {
        personList.append(Person(p.username));
    }
    return personList;
}

void SessionManager::onFileTransferAccepted(const QString& senderUsername, const QString& fileName) {

}
