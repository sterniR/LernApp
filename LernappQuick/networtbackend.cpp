#include "networtbackend.h"
#include "curl/curl.h"
#include <qregularexpression.h>
#include <QDebug>

NetwortBackend::NetwortBackend(QObject *parent)
    : QObject{parent}
{}



QStringList NetwortBackend::getFTPFileList(const QString& ftpUrl, const QString& username, const QString& password) {
    QStringList fileList;
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.toStdString().c_str());

        QString userPwd = username + ":" + password;
        curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.toStdString().c_str());

        // FTP Listing abrufen
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NetwortBackend::getDirFtp);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileList);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl Fehler: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return fileList;
}

size_t NetwortBackend::getDirFtp(void *contents, size_t size, size_t nmemb, void *userp) // libcurl -
{
    QStringList* fileList = static_cast<QStringList*>(userp);
    QString data = QString::fromUtf8(static_cast<char*>(contents), size * nmemb);

    fileList->append(parseFTPList(data));

    return size * nmemb;
}

QStringList NetwortBackend::parseFTPList(const QString &response) { //Output-Datei filtern mit Regex
    QStringList fileList;

    // Regulärer Ausdruck für Dateinamen
    QRegularExpression regex(R"(\S+\s+\d+\s+\d+\s+\d+\s+\d+\s+\w+\s+\d+\s+[\d:]+\s+(.+))");

    QRegularExpressionMatchIterator i = regex.globalMatch(response);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            fileList.append(match.captured(1));  // Der Dateiname
        }
    }
    return fileList;
}

void NetwortBackend::refreshServer() // Server-Dateien Aktualisieren/Fetchen
{
    // FTP-Dateien abrufen
    QString ftpUrl = "ftp://138.199.195.70:21/files/";
    QString username = "bob";
    QString password = "Kartoffel123?!";
    QStringList files = NetwortBackend::getFTPFileList(ftpUrl, username, password);

    m_dataFileFromFtpServer.clear();
    for (const QString& file : files) {
        m_dataFileFromFtpServer.append(file);
        qDebug() << m_dataFileFromFtpServer;
    }
    emit dataFileFromFtpServerChanged();
}

void NetwortBackend::ThemeDatabaseSelected(const QString &db)
{
    selectedDatabase = db;
    qDebug() << selectedDatabase << " db";
    m_nameDatabaseSelected = db;
    qDebug() << selectedDatabase << " qml file";
    emit nameDatabaseSelectedChanged();
}

QStringList NetwortBackend::dataFileFromFtpServer() const //dataFileFromFtpServer
{
    return m_dataFileFromFtpServer;
}

void NetwortBackend::setDataFileFromFtpServer(const QStringList &newDataFileFromFtpServer)
{
    if (m_dataFileFromFtpServer == newDataFileFromFtpServer)
        return;
    m_dataFileFromFtpServer = newDataFileFromFtpServer;
    emit dataFileFromFtpServerChanged();
}

QString NetwortBackend::nameDatabaseSelected() const //nameDatabaseSelected
{
    return m_nameDatabaseSelected;
}

void NetwortBackend::setNameDatabaseSelected(const QString &newNameDatabaseSelected)
{
    if (m_nameDatabaseSelected == newNameDatabaseSelected)
        return;
    m_nameDatabaseSelected = newNameDatabaseSelected;
    emit nameDatabaseSelectedChanged();
}
