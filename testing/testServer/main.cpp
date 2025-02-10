#include <QCoreApplication>
#include <QThread>
#include "ftpsclient.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    FtpsClient client;
    client.connectToServer("deinserver.com", 21);

    // Warte kurz, bis die Verbindung aufgebaut ist
    QThread::sleep(2);

    client.login("benutzername", "passwort");
    QThread::sleep(2);

    client.listDirectory();

    return a.exec();
}
