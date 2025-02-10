#include "ftpsclient.h"
#include <QDebug>

FtpsClient::FtpsClient(QObject *parent) : QObject(parent), socket(new QSslSocket(this)) {
    connect(socket, &QSslSocket::connected, this, &FtpsClient::onConnected);
    connect(socket, &QSslSocket::readyRead, this, &FtpsClient::onReadyRead);
    connect(socket, &QSslSocket::encrypted, this, &FtpsClient::onEncrypted);
    connect(socket, &QSslSocket::sslErrors, this, &FtpsClient::onSslErrors);
}

FtpsClient::~FtpsClient() {
    socket->disconnectFromHost();
}

void FtpsClient::connectToServer(const QString &host, quint16 port) {
    qDebug() << "Verbinde mit" << host << "auf Port" << port;
    socket->connectToHostEncrypted(host, port);
}

void FtpsClient::login(const QString &username, const QString &password) {
    if (!socket->isEncrypted()) {
        qDebug() << "Verbindung ist nicht verschlüsselt!";
        return;
    }

    sendCommand("USER " + username);
    sendCommand("PASS " + password);
}

void FtpsClient::listDirectory() {
    if (!isLoggedIn) {
        qDebug() << "Nicht eingeloggt!";
        return;
    }

    sendCommand("PASV");  // Passiven Modus für Datenverbindung
    sendCommand("LIST");  // Verzeichnisinhalt abrufen
}

void FtpsClient::onConnected() {
    qDebug() << "Verbunden mit dem Server!";
}

void FtpsClient::onEncrypted() {
    qDebug() << "SSL-Verschlüsselung aktiviert!";
}

void FtpsClient::onSslErrors(const QList<QSslError> &errors) {
    for (const auto &error : errors) {
        qDebug() << "SSL-Fehler:" << error.errorString();
    }
}

void FtpsClient::onReadyRead() {
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        responseBuffer.append(line);
        qDebug() << "Serverantwort:" << line;

        if (line.startsWith("230")) {
            isLoggedIn = true;
            qDebug() << "Login erfolgreich!";
        }
    }
}

void FtpsClient::sendCommand(const QString &command) {
    if (socket->isWritable()) {
        socket->write((command + "\r\n").toUtf8());
        socket->flush();
        qDebug() << "Gesendet:" << command;
    }
}
