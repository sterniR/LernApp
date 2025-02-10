#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QSslSocket(this))
{
    ui->setupUi(this);
    connect(socket, &QSslSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QSslSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QSslSocket::encrypted, this, &MainWindow::onEncrypted);
    connect(socket, &QSslSocket::sslErrors, this, &MainWindow::onSslErrors);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead() {
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        responseBuffer.append(line);
        qDebug() << "Serverantwort:" << line;

        if (line.startsWith("220")) {
            qDebug() << "Server bereit. Sende AUTH TLS...";
            sendCommand("AUTH TLS");
        }
        else if (line.startsWith("234")) {
            qDebug() << "TLS akzeptiert. Starte Verschlüsselung...";
            socket->startClientEncryption();
        }
        else if (line.startsWith("331")) {
            qDebug() << "Benutzername akzeptiert, Passwort wird gesendet...";
            sendCommand("PASS " + password);
        }
        else if (line.startsWith("230")) {
            isLoggedIn = true;
            qDebug() << "Login erfolgreich!";
        }
        else if (line.startsWith("530")) {
            qDebug() << "Login fehlgeschlagen! Falscher Benutzername oder Passwort?";
        }
    }
}

void MainWindow::sendCommand(const QString &command) {
    if (socket->isWritable()) {
        socket->write((command + "\r\n").toUtf8());
        socket->flush();
        qDebug() << "Gesendet:" << command;
    }
}


void MainWindow::onConnected() {
    qDebug() << "Verbunden mit dem Server!";
}

void MainWindow::listDirectory() {
    if (!isLoggedIn) {
        qDebug() << "Nicht eingeloggt!";
        return;
    }

    sendCommand("PASV");  // Passiven Modus für Datenverbindung
    sendCommand("LIST");  // Verzeichnisinhalt abrufen
}

void MainWindow::login(const QString &username, const QString &password) {
    // if (!socket->isEncrypted()) {
    //     qDebug() << "Verbindung ist nicht verschlüsselt!";
    //     return;
    // }

    sendCommand("USER " + username);
    sendCommand("PASS " + password);
}

void MainWindow::connectToServer(const QString &host, quint16 port) {
    qDebug() << "Connect to: " << host << " with Port: " << port;
    socket->connectToHostEncrypted(host,port);
    if (!socket->waitForConnected(5000)) {
        qDebug() << "Verbindungsfehler:" << socket->errorString();
        return;
    }
}

void MainWindow::on_pushButton_connect_clicked()
{
    connectToServer("138.199.195.70",21);
}

void MainWindow::onEncrypted() {
    qDebug() << "SSL-Verschlüsselung aktiviert!";
}

void MainWindow::onSslErrors(const QList<QSslError> &errors) {
    for (const auto &error : errors) {
        qDebug() << "SSL-Fehler:" << error.errorString();
    }
}
