#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.connectToServer("138.199.195.70", 21);

    // Warte kurz, bis die Verbindung aufgebaut ist
    QThread::sleep(2);

    w.login("bob", "Kartoffel123?!");
    QThread::sleep(2);

    w.listDirectory();

    QThread::sleep(2);

    // w.show();
    return a.exec();
}
