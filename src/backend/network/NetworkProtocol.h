#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H


#include <QObject>
#include <QVariant>


constexpr quint16 UDP_PORT_HOST = 8585;
constexpr quint16 TCP_PORT_HOST = 8586;
constexpr quint16 UDP_PORT_CLIENT = 8687;
constexpr quint16 TCP_PORT_CLIENT = 8688;

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
    SongResponsePositive,
    SongResponseNegative,

    FileTransferRequest,
    FileTransferResponse,
    FileReceivedAck,

    FileTransferStart,
    FileTransferProgress,
    FileTransferFinished,
    FileTransferError,

    KickUser,
    UserKicked

};

Q_DECLARE_METATYPE(NetworkCommand)


using ParticipantInfo = QMap<QString, QString>;

Q_DECLARE_METATYPE(ParticipantInfo)

#endif // NETWORKPROTOCOL_H
