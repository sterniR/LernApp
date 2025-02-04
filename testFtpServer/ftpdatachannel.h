#ifndef FTPDATACHANNEL_H
#define FTPDATACHANNEL_H

#include <QtCore/qobject.h>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <memory>

class FtpDataChannel : public QObject
{
    Q_OBJECT
public:
    explicit FtpDataChannel(QObject *parent = nullptr);
    void listen(const QHostAddress &address = QHostAddress::Any);
    void sendData(const QByteArray &data);
    void close();
    QString portspec() const;
    QTcpServer m_server;
    std::unique_ptr<QTcpSocket> m_socket;

signals:
    void dataReceived(const QByteArray &data);
};

#endif // FTPDATACHANNEL_H
