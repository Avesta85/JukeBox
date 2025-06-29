#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H


#include <QObject>
#include <QVariant>


constexpr quint16 UDP_PORT = 8585;
constexpr quint16 TCP_PORT = 85856;

enum class NetworkCommand :qint32 {

    Invalid =0,
    SessionJoinRequest,
    SessionJoinAccept,
    SessionJoinReject,
    SessionLeave,
    ParticipantListUpdate,

    ChatMessage,

    Play,
    Pause,
    Seek,

    CheckSongRequest,
    CheckSongResponse,

    FileTransferRequest,
    FileTransferResponse

};

Q_DECLARE_METATYPE(NetworkCommand)


using ParticipantInfo = QMap<QString, QString>;

Q_DECLARE_METATYPE(ParticipantInfo)
#endif // NETWORKPROTOCOL_H
