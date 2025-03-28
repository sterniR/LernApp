#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSslSocket>
#include <QStringList>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connectToServer(const QString &host, quint16 port);
    void login(const QString &username, const QString &password);
    void listDirectory();
    QString password = "Kartoffel123?!";

private slots:
    void onReadyRead();
    void onEncrypted();
    void onConnected();
    void onSslErrors(const QList<QSslError> &errors);

    void on_pushButton_connect_clicked();

private:
    void sendCommand(const QString &command);

    Ui::MainWindow *ui;
    QSslSocket *socket;
    QStringList responseBuffer;
    bool isLoggedIn = false;
};
#endif // MAINWINDOW_H
