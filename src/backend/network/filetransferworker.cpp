#include "filetransferworker.h"


FileTransferWorker::FileTransferWorker(const QString &filePath, const QHostAddress &address, quint16 port, QObject *parent)
    : QObject{parent},
    m_filePath(filePath),
    m_targetAddress(address),
    m_targetPort(port),
    m_socket(nullptr),
    m_totalBytes(0),
    m_bytesWritten(0)
{

}
