#ifndef FTPSCLIENT_H
#define FTPSCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QStringList>

class FtpsClient : public QObject {
    Q_OBJECT

public:
    explicit FtpsClient(QObject *parent = nullptr);
    ~FtpsClient();

    void connectToServer(const QString &host, quint16 port);
    void login(const QString &username, const QString &password);
    void listDirectory();

private slots:
    void onReadyRead();
    void onEncrypted();
    void onConnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    void sendCommand(const QString &command);

    QSslSocket *socket;
    QStringList responseBuffer;
    bool isLoggedIn = false;
};

#endif // FTPSCLIENT_H
