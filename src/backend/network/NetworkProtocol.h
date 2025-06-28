#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H


#include <QObject>
#include <QVariant>


enum class NetworkCommand :qint32 {

    Invalid =0,
    ChatMessage,

    Play,
    Pause,
    Seek,

    CheckSongRequest,
    CheckSongResponse,
    SendFileRequest,
    SendFileAccept,
    SendFileReject

};

Q_DECLARE_METATYPE(NetworkCommand)

#endif // NETWORKPROTOCOL_H
