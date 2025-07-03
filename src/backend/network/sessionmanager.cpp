#include "sessionmanager.h"
#include <QDir>
#include "ConnectionManager.h"
#include "qmessagebox.h"
#include "qstandardpaths.h"
#include <src/backend/db/DBM.h>


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


    ConnectionManager::getInstance().startListening(m_isHost);
    emit participantListChanged(participantsAsPersonList());
    emit showInfoMessage("New session created. Waiting for friends to join...");
}

void SessionManager::joinSession(const QHostAddress &hostAddress,const QString &myUsername)
{
    leaveSession();
    ConnectionManager::getInstance().startListening(m_isHost);
    m_isSessionActive = true;
    m_isHost = false;
    m_localUser = {myUsername,QHostAddress(),0};
    m_hostAddress = hostAddress;

    QVariantMap payload;

    payload["username"] = myUsername;
    ConnectionManager::getInstance().sendCommand(hostAddress,UDP_PORT_HOST,NetworkCommand::SessionJoinRequest,payload);
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

    QString songname = song.getName();

    m_pendingSongRequest.remove(songname);


    QStringList needingUsers;

    for(const auto& p : m_participants){
        if(p.username != m_localUser.username){
            needingUsers.append(p.username);
        }
    }

    if (needingUsers.isEmpty()){
        qDebug() << "No other participants. Playing song directly.";
        return;
    }
    m_pendingSongRequest.insert(songname,needingUsers);


    broadcastCommand(NetworkCommand::CheckSongRequest,songname);

    m_checkSongResponseTimer->start();
    emit showInfoMessage("Asking others if they have the song: " + songname);

}

void SessionManager::onChatMessageSendRequested(const QString &message)
{
    if(!m_isSessionActive || message.isEmpty()) return;
    if (m_isHost) {
        emit newChatMessageForUI("Me: "+message);
        for(const Participant& p : m_participants) {
            if(p.username != m_localUser.username) {
                ConnectionManager::getInstance().sendCommand(p.address, UDP_PORT_CLIENT, NetworkCommand::ChatMessage, message);
            }
        }
    } else {
        sendChatMessageToHost(message);
        emit newChatMessageForUI("Me: "+message); // نمایش فوری برای خود کاربر
    }
}

void SessionManager::sendChatMessageToHost(const QString& message)
{
    if (!m_isSessionActive || m_isHost) return;
    ConnectionManager::getInstance().sendCommand(m_hostAddress, UDP_PORT_HOST, NetworkCommand::ChatMessage, message);
    emit newChatMessageForUI("Me: "+message);
}

void SessionManager::processNetworkCommand(NetworkCommand command, const QVariant &payload, const QHostAddress &sender, quint16 senderPort)
{
    if (!m_isSessionActive) return;

    if (m_isHost)
    {
        if (command == NetworkCommand::SessionJoinRequest) {
            QString newUsername = payload.toMap().value("username").toString();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Join Request");
            msgBox.setText(QString("%1 wants to join the session. Accept?").arg(newUsername));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes) {
                Participant newParticipant = {newUsername, sender, senderPort};
                m_participants.append(newParticipant);
                QVariantMap acceptPayload;
                acceptPayload["hostUsername"] = m_localUser.username;
                ConnectionManager::getInstance().sendCommand(sender, UDP_PORT_CLIENT, NetworkCommand::SessionJoinAccept, acceptPayload);
                emit showInfoMessage(QString("%1 has joined the session.").arg(newUsername));
                broadcastParticipantList();
                emit participantListChanged(participantsAsPersonList());
            } else {
                ConnectionManager::getInstance().sendCommand(sender, UDP_PORT_CLIENT, NetworkCommand::SessionJoinReject, QVariant());
                emit showInfoMessage(QString("Join request from %1 was rejected.").arg(newUsername));
            }
        }
        else if (command == NetworkCommand::CheckSongResponse) {
            QVariantMap map = payload.toMap();
            QString responseSongName = map.value("songName").toString();
            bool hasSong = map.value("hasSong").toBool();
            QString senderUsername = findUsernameByAddress(sender);
            if (hasSong) {
                if (m_pendingSongRequest.contains(responseSongName)) {
                    m_pendingSongRequest[responseSongName].removeAll(senderUsername);
                }
            }
            if (m_pendingSongRequest.contains(responseSongName) && m_pendingSongRequest[responseSongName].isEmpty()) {
                m_pendingSongRequest.remove(responseSongName);
                emit songsynced();
            }
        }
        else if (command == NetworkCommand::SongResponsePositive) {
            QVariantMap map = payload.toMap();
            QString responseSongName = map.value("songName").toString();
            QString senderUsername = findUsernameByAddress(sender);
            if (m_pendingSongRequest.contains(responseSongName)) {
                m_pendingSongRequest[responseSongName].removeAll(senderUsername);
                if (m_pendingSongRequest[responseSongName].isEmpty()) {
                    m_pendingSongRequest.remove(responseSongName);
                    emit songsynced();
                }
            }
        }
        else if (command == NetworkCommand::SongResponseNegative) {
            QVariantMap map = payload.toMap();
            QString responseSongName = map.value("songName").toString();
            QString senderUsername = findUsernameByAddress(sender);
            if (!m_pendingSongRequest.contains(responseSongName)) {
                m_pendingSongRequest[responseSongName] = QStringList();
            }
            if (!m_pendingSongRequest[responseSongName].contains(senderUsername)) {
                m_pendingSongRequest[responseSongName].append(senderUsername);
            }
            qDebug() << "[Sync] User" << senderUsername << "does not have song" << responseSongName << ". Will send file.";
        }
        else if (command == NetworkCommand::FileTransferRequest) {
            QVariantMap map = payload.toMap();
            QString fileName = map.value("fileName").toString();
            auto song = DBM::get_instance().getSongByName(fileName);
            QString senderUsername = findUsernameByAddress(sender);
            QString filePath = song.getPath();
            qDebug() << "[FileTransfer][Host] Sending file" << filePath << "to user" << senderUsername;
            ConnectionManager::getInstance().sendFile(filePath, sender, TCP_PORT_CLIENT);
            emit showInfoMessage(QString("Started file transfer to %1 for file %2").arg(senderUsername, fileName));
        }
        else if (command == NetworkCommand::FileReceivedAck) {
            QVariantMap map = payload.toMap();
            QString fileName = map.value("fileName").toString();
            QString senderUsername = findUsernameByAddress(sender);
            if (m_pendingSongRequest.contains(fileName)) {
                m_pendingSongRequest[fileName].removeAll(senderUsername);
                if (m_pendingSongRequest[fileName].isEmpty()) {
                    m_pendingSongRequest.remove(fileName);
                    emit songsynced();
                }
            }
        }
        else if (command == NetworkCommand::FileTransferStart) {
            emit showInfoMessage("File transfer started.");
            emit syncStatusChanged("File transfer started...");
            qDebug() << "[FileTransfer][Host] File transfer started.";
        }
        else if (command == NetworkCommand::FileTransferProgress) {
            int percent = payload.toInt();
            emit showInfoMessage(QString("File transfer progress: %1%").arg(percent));
            emit syncStatusChanged(QString("%1 percent of file transferred").arg(percent));
            qDebug() << "[FileTransfer][Host] Progress:" << percent << "%";
        }
        else if (command == NetworkCommand::FileTransferFinished) {
            emit showInfoMessage("File transfer finished.");
            emit syncStatusChanged("File transfer finished successfully.");
            qDebug() << "[FileTransfer][Host] File transfer finished.";
        }
        else if (command == NetworkCommand::FileTransferError) {
            QString err = payload.toString();
            emit showInfoMessage(QString("File transfer error: %1").arg(err));
            emit syncError(QString("File transfer error: %1").arg(err));
            qDebug() << "[FileTransfer][Host][Error]" << err;
        }
    }
    else // CLIENT
    {
        if (command == NetworkCommand::ParticipantListUpdate) {
            m_participants.clear();
            QVariantList list = payload.toList();
            for (const QVariant& item : list) {
                QVariantMap map = item.toMap();
                Participant p = {map["username"].toString(), QHostAddress(map["address"].toString()), static_cast<quint16>(map["port"].toUInt())};
                m_participants.append(p);
            }
            emit participantListChanged(participantsAsPersonList());
        }
        else if (command == NetworkCommand::CheckSongRequest) {
            QString songName = payload.toString();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Sync Song");
            msgBox.setText(QString("Do you have the song '%1'?\nClick Yes if you have it, No if you don't.").arg(songName));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();
            bool hasSong = (ret == QMessageBox::Yes);
            QVariantMap responsePayload;
            responsePayload["songName"] = songName;
            if (hasSong) {
                ConnectionManager::getInstance().sendCommand(m_hostAddress, UDP_PORT_HOST, NetworkCommand::SongResponsePositive, responsePayload);
                Song song = DBM::get_instance().getSongByName(songName);
                emit setCurrentSong(song);
            } else {
                ConnectionManager::getInstance().sendCommand(m_hostAddress, UDP_PORT_HOST, NetworkCommand::SongResponseNegative, responsePayload);
                QVariantMap fileReqPayload;
                fileReqPayload["fileName"] = songName;
                ConnectionManager::getInstance().sendCommand(m_hostAddress, UDP_PORT_HOST, NetworkCommand::FileTransferRequest, fileReqPayload);
                emit syncStatusChanged("Requested file from host...");
                qDebug() << "[FileTransfer][Client] Requested file from host for" << songName;
            }
        }
        else if (command == NetworkCommand::SessionJoinAccept) {
            QString hostUsername = payload.toMap().value("hostUsername").toString();
            m_hostUsername = hostUsername;
            QMessageBox::information(nullptr, "Host", "Welcome... !");
            emit joinRequestaccept();
        }
        else if (command == NetworkCommand::SessionJoinReject) {
            emit joinRequestReject();
        }
        else if (command == NetworkCommand::FileTransferStart) {
            emit showInfoMessage("File transfer started.");
            emit syncStatusChanged("File transfer started...");
            qDebug() << "[FileTransfer][Client] File transfer started.";
        }
        else if (command == NetworkCommand::FileTransferProgress) {
            int percent = payload.toInt();
            emit showInfoMessage(QString("File transfer progress: %1%").arg(percent));
            emit syncStatusChanged(QString("%1 percent of file transferred").arg(percent));
            qDebug() << "[FileTransfer][Client] Progress:" << percent << "%";
        }
        else if (command == NetworkCommand::FileTransferFinished) {
            emit showInfoMessage("File transfer finished.");
            emit syncStatusChanged("File transfer finished successfully.");
            qDebug() << "[FileTransfer][Client] File transfer finished.";
        }
        else if (command == NetworkCommand::FileTransferError) {
            QString err = payload.toString();
            emit showInfoMessage(QString("File transfer error: %1").arg(err));
            emit syncError(QString("File transfer error: %1").arg(err));
            qDebug() << "[FileTransfer][Client][Error]" << err;
        }
    }

    if (command == NetworkCommand::ChatMessage) {
        QString message = payload.toString();
        QString senderUsername;
        if (m_isHost) {
            senderUsername = findUsernameByAddress(sender);
        } else {
            if (sender == m_hostAddress) {
                senderUsername = m_hostUsername.isEmpty() ? "Host" : m_hostUsername;
            } else {
                senderUsername = findUsernameByAddress(sender);
            }
        }
        emit newChatMessageForUI(QString("%1: %2").arg(senderUsername, message));
    }
    else if (command == NetworkCommand::KickUser) {
        if (!m_isHost) {
            QVariantMap map = payload.toMap();
            QString kickedUsername = map.value("username").toString();
            QString reason = map.value("reason").toString();
            emit kickedFromSession(reason);
            emit showInfoMessage(QString("You have been kicked from the session. Reason: %1").arg(reason));
            leaveSession();
        }
    }
    else if (command == NetworkCommand::UserKicked) {
        QVariantMap map = payload.toMap();
        QString kickedUsername = map.value("username").toString();
        emit systemMessage(QString("%1 has been kicked from the session.").arg(kickedUsername));
        emit showInfoMessage(QString("%1 has been kicked from the session.").arg(kickedUsername));
    }
    else if (command == NetworkCommand::Play) {
        qint64 pos = payload.toLongLong();
        emit remotePlayRequested(pos);
    } else if (command == NetworkCommand::Pause) {
        emit remotePauseRequested();
    } else if (command == NetworkCommand::Seek) {
        qint64 pos = payload.toLongLong();
        emit remoteSeekRequested(pos);
    }
    else if (command == NetworkCommand::SessionLeave) {
        QString leavingUsername = payload.toString();
        auto it = std::remove_if(m_participants.begin(), m_participants.end(),
                                 [&](const Participant& p){ return p.username == leavingUsername; });
        if (it != m_participants.end()) {
            m_participants.erase(it, m_participants.end());
            emit participantListChanged(participantsAsPersonList());
            broadcastParticipantList();
        }
    }
}

void SessionManager::onCheckSongResponseTimeout()
{
    static QMap<QString, int> timeoutCounts; // songName -> timeout count
    const int MAX_TIMEOUTS = 3;

    while (!m_pendingSongRequest.isEmpty()) {
        QString songName = m_pendingSongRequest.firstKey();
        Song tmp = DBM::get_instance().getSongByName(songName);
        QStringList usersToSync = m_pendingSongRequest.value(songName);
        m_pendingSongRequest.remove(songName);

        if (usersToSync.isEmpty()) {
            emit songsynced();
            qDebug() << "[Sync] No users to sync for song" << songName << ". songsynced emitted.";
            continue;
        }

        int& timeoutCount = timeoutCounts[songName];
        timeoutCount++;

        if (timeoutCount > MAX_TIMEOUTS) {
            emit syncError(QString("Sync failed for song '%1': too many timeouts. Skipping these users: %2").arg(songName, usersToSync.join(", ")));
            qDebug() << "[Sync][Error] Too many timeouts for song" << songName << ". Skipping users:" << usersToSync;
            timeoutCounts.remove(songName);
            continue;
        }

        for (const QString& username : usersToSync) {
            Participant targetParticipant;
            bool found = false;
            for (const auto& p : m_participants) {
                if (p.username == username) {
                    targetParticipant = p;
                    found = true;
                    break;
                }
            }
            if (found) {
                QString filePath =tmp.getPath();
                qDebug() << "[Sync] Starting file transfer of" << filePath << "to" << username;
                ConnectionManager::getInstance().sendFile(filePath, targetParticipant.address, TCP_PORT_CLIENT);
            } else {
                qDebug() << "[Sync][Error] Could not find participant" << username << "to send file.";
                emit syncError(QString("Could not find participant '%1' to send file '%2'").arg(username, songName));
            }
        }
        break;
    }
    if (m_pendingSongRequest.isEmpty()) {
        timeoutCounts.clear();
    }
}

SessionManager::SessionManager(QObject *parent)
    : QObject{parent},
    m_isSessionActive(false),
    m_isHost(false)
{
    connect(&ConnectionManager::getInstance(), &ConnectionManager::commandReceived,
            this, &SessionManager::processNetworkCommand);


    m_checkSongResponseTimer = new QTimer(this);
    m_checkSongResponseTimer->setSingleShot(true);
    m_checkSongResponseTimer->setInterval(5000);
    connect(m_checkSongResponseTimer, &QTimer::timeout, this, &SessionManager::onCheckSongResponseTimeout);
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
            ConnectionManager::getInstance().sendCommand(p.address,UDP_PORT_CLIENT,command,payload);
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
    if (!m_isHost && senderUsername == m_localUser.username) {
        emit syncStatusChanged("Song received successfully. Loading into library...");
        qDebug() << "[FileTransfer] File received for client:" << fileName;
        QMessageBox msgBox;
        msgBox.setWindowTitle("File Received");
        msgBox.setText(QString("File '%1' received successfully! Click OK to continue.").arg(fileName));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        QVariantMap ackPayload;
        ackPayload["fileName"] = fileName;
        ConnectionManager::getInstance().sendCommand(m_hostAddress, UDP_PORT_HOST, NetworkCommand::FileReceivedAck, ackPayload);

        DBM::get_instance().Reload_folder();
        Song song = DBM::get_instance().getSongByName(fileName);
        if (song.getPath().isEmpty()) {
            emit syncError("Song received but not found in database!");
            qDebug() << "[FileTransfer][Error] Song not found in DB after transfer:" << fileName;
        } else {
            emit setCurrentSong(song);
            qDebug() << "[FileTransfer] Song set as current after transfer:" << song.getName();
        }
    }
    if(m_pendingSongRequest.contains(fileName)) {
        m_pendingSongRequest[fileName].removeAll(senderUsername);
        qDebug() << "[FileTransfer][Host] Ack received from user:" << senderUsername << "for file:" << fileName;
        if(m_pendingSongRequest[fileName].isEmpty()) {
            m_pendingSongRequest.remove(fileName);
            emit songsynced();
            qDebug() << "[FileTransfer][Host] All users have received the file. songsynced emitted.";
        }
    }
}

void SessionManager::kickUser(const QString& username)
{
    if (!m_isSessionActive || !m_isHost) return;

    Participant targetParticipant;
    bool found = false;
    for (const auto& p : m_participants) {
        if (p.username == username) {
            targetParticipant = p;
            found = true;
            break;
        }
    }

    if (!found) {
        qDebug() << "User" << username << "not found in session";
        return;
    }

    QVariantMap payload;
    payload["username"] = username;
    payload["reason"] = "Kicked by host";
    
    ConnectionManager::getInstance().sendCommand(targetParticipant.address, UDP_PORT_CLIENT, 
                                                NetworkCommand::KickUser, payload);

    m_participants.removeAll(targetParticipant);
    
    broadcastParticipantList();
    emit participantListChanged(participantsAsPersonList());
    
    emit showInfoMessage(QString("%1 has been kicked from the session.").arg(username));
}

void SessionManager::kickUsers(const QList<Person>& users)
{
    if (!m_isSessionActive || !m_isHost) return;

    for (const auto& user : users) {
        kickUser(user.getUserName());
    }
}

bool SessionManager::getSessioonActive()
{
    return m_isSessionActive;
}

bool SessionManager::ISHost()
{
    return m_isHost;
}

void SessionManager::broadcastPlayCommand(qint64 position) {
    if (m_isSessionActive && m_isHost) {
        broadcastCommand(NetworkCommand::Play, position);
    }
}

void SessionManager::RequestsyncSong(const Song& song)
{
    if (!m_isSessionActive) return;

    if (m_isHost) {
        QString songname = song.getName();
        m_pendingSongRequest.remove(songname);

        QStringList needingUsers;
        for(const auto& p : m_participants){
            if(p.username != m_localUser.username){
                needingUsers.append(p.username);
            }
        }

        if (needingUsers.isEmpty()){
            emit songsynced();
            return;
        }
        m_pendingSongRequest.insert(songname, needingUsers);

        broadcastCommand(NetworkCommand::CheckSongRequest, songname);

        m_checkSongResponseTimer->start();
        emit showInfoMessage("Asking others if they have the song: " + songname);
    }
}


void SessionManager::broadcastPauseCommand() {
    if (m_isSessionActive && m_isHost) {
        broadcastCommand(NetworkCommand::Pause, QVariant());
    }
}
void SessionManager::broadcastSeekCommand(qint64 position) {
    if (m_isSessionActive && m_isHost) {
        broadcastCommand(NetworkCommand::Seek, position);
    }
}

void SessionManager::sendLeaveRequestToHost()
{
    if (!m_isHost && m_isSessionActive) {
        ConnectionManager::getInstance().sendCommand(m_hostAddress, UDP_PORT_HOST, NetworkCommand::SessionLeave, m_localUser.username);
    }
}



