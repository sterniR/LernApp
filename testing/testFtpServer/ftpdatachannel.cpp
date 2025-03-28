#include "ftpdatachannel.h"

FtpDataChannel::FtpDataChannel(QObject *parent) :
    QObject(parent)
{
    connect(&m_server, &QTcpServer::newConnection, this, [this]() {
        m_socket.reset(m_server.nextPendingConnection());
        connect(m_socket.get(), &QTcpSocket::readyRead, this, [this]() {
            emit dataReceived(m_socket->readAll());
        });

        connect(m_socket.get(), &QTcpSocket::bytesWritten, this, [this](qint64 bytes) {
            qDebug() << bytes;
            close();
        });
    });
}

void FtpDataChannel::listen(const QHostAddress &address) {
    m_server.listen(address);
}

void FtpDataChannel::sendData(const QByteArray &data) {
    if(m_socket)
        m_socket->write(QByteArray(data).replace("\n", "\r\n"));
}

void FtpDataChannel::close() {
    if(m_socket)
        m_socket->disconnectFromHost();
}

QString FtpDataChannel::portspec() const {
    QString portSpec;
    quint32 ipv4 = m_server.serverAddress().toIPv4Address();
    quint16 port = m_server.serverPort();
    portSpec += QString::number((ipv4 & 0xff000000) >> 24);
    portSpec += ',' + QString::number((ipv4 & 0x00ff0000) >> 16);
    portSpec += ',' + QString::number((ipv4 & 0x0000ff00) >> 8);
    portSpec += ',' + QString::number(ipv4 & 0x000000ff);
    portSpec += ',' + QString::number((port & 0xff00) >> 8);
    portSpec += ',' + QString::number(port & 0x00ff);
    return portSpec;
}
