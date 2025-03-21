#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QQmlEngine>

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Backend(QObject *parent = nullptr);

public slots:
    static QStringList getFTPFileList(const QString& ftpUrl, const QString& username, const QString& password);
    static size_t getDirFtp(void* contents, size_t size, size_t nmemb, void* userp);
    static QStringList parseFTPList(const QString &response);
    void refreshServer();
signals:
};

#endif // BACKEND_H
