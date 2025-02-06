#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QThread>
#include "ftpcontrolchannel.h"
#include "ftpdatachannel.h"

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
    void getFileList();

private slots:
    void on_openButton_clicked();

    void on_uploadButton_clicked();

    void on_setFolderButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void serverConnected(const QHostAddress &address, int port);

    void serverReply(int code, const QString &parameter);

    void dataReceived(const QByteArray &data);

private:
    Ui::MainWindow *ui;
    FtpDataChannel *dataChannel;
    FtpControlChannel * controlChannel;
    QString ftpAddress, username, password, uploadFileName, downloadFileName;
    QStringList fileList;
};
#endif // MAINWINDOW_H
