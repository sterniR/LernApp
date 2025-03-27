#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>
#include <QStandardPaths>

class Network : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QStringList& dataFileFromFtpServer READ dataFileFromFtpServer WRITE setDataFileFromFtpServer NOTIFY dataFileFromFtpServerChanged FINAL)
    Q_PROPERTY(QString nameDatabaseSelected READ nameDatabaseSelected WRITE setNameDatabaseSelected NOTIFY nameDatabaseSelectedChanged FINAL)
    QML_ELEMENT
public:
    explicit Network(QObject *parent = nullptr);

    Q_INVOKABLE void ThemeDatabaseSelected(const QString& db);
    Q_INVOKABLE void downloadFile();

    QString const pathSystem = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QStringList dataFileFromFtpServer() const;
    void setDataFileFromFtpServer(const QStringList &newDataFileFromFtpServer);

    QString nameDatabaseSelected() const;
    void setNameDatabaseSelected(const QString &newNameDatabaseSelected);

public slots:
    static QStringList getFTPFileList(const QString& ftpUrl, const QString& username, const QString& password);
    static size_t getDirFtp(void* contents, size_t size, size_t nmemb, void* userp);
    static QStringList parseFTPList(const QString &response);
    static size_t WriteCallBack(void *contents, size_t size, size_t nmemb, void *userp);
    void refreshServer();

signals:
    void dataFileFromFtpServerChanged();

    void nameDatabaseSelectedChanged();

private:
    QStringList m_dataFileFromFtpServer;
    QString m_nameDatabaseSelected;
};
#endif // NETWORK_H
