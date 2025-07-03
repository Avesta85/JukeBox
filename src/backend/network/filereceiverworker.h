#ifndef FILERECEIVERWORKER_H
#define FILERECEIVERWORKER_H

#include "qdir.h"
#include <QObject>

class QTcpSocket;
class QFile;

class FileReceiverWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiverWorker(QTcpSocket* socket, const QString& savePath,QObject *parent = nullptr);
    ~FileReceiverWorker();



public slots:
    void startReceiving();

signals:
    void progress(int percentage);
    void finished(const QString& savedFilePath);
    void error(const QString& errorString);
private slots :
    void onReadyRead();
    void onDisconnected();

private :
    QTcpSocket* m_socket;
    QFile m_file;
    QString m_savePath;
    qint64 m_totalbytes=0;
    qint64 m_bytesReceived =0;
    QString m_fileName;
};

#endif // FILERECEIVERWORKER_H

