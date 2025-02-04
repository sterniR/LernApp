#ifndef FTPCONTROLCHANNEL_H
#define FTPCONTROLCHANNEL_H

#include <QtCore/qobject.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qtcpsocket.h>

class FtpControlChannel : public QObject
{
    Q_OBJECT
public:
    explicit FtpControlChannel(QObject *parent = nullptr);
    void connectToServer(const QString &server);
    void command(const QByteArray &command, const QByteArray &params);
public slots:
    void error(QAbstractSocket::SocketError);
signals:
    void closed();
    void info(const QByteArray &info);
    void reply(int code, const QByteArray &parameters);
    void invalidReply(const QByteArray &reply);
private:
    void onReadyRead();
    QTcpSocket m_socket;
    QByteArray m_buffer;
};

#endif // FTPCONTROLCHANNEL_H
