#ifndef FILETRANSFERWORKER_H
#define FILETRANSFERWORKER_H

#include <QObject>
#include <QObject>
#include <QString>
#include <QHostAddress>

class QTcpSocket;

class FileTransferWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileTransferWorker(const QString& filePath, const QHostAddress& address, quint16 port, QObject* parent = nullptr);


public slots:
    void startTransfer();

signals:
    void progress(const QString fileName, int percentage);
    void finished(const QString fileName);
    void error(const QString& errormessage);

private slots :
    void onConnected();
    void onByteWritten(qint64 bytes);
    void onSocketError();


private:
    QString m_filePath;
    QHostAddress m_targetAddress;
    quint16 m_targetPort;
    QTcpSocket* m_socket;
    qint64 m_totalBytes = 0;
    qint64 m_bytesWritten = 0;
};

#endif // FILETRANSFERWORKER_H

