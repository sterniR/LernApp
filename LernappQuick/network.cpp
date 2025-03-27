#include "network.h"
#include "curl/curl.h"
#include <qregularexpression.h>

#include <QDebug>
#include <fstream>
#include <iostream>

Network::Network(QObject *parent)
    : QObject{parent}
{}



QStringList Network::getFTPFileList(const QString& ftpUrl, const QString& username, const QString& password) {
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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Network::getDirFtp);
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

size_t Network::getDirFtp(void *contents, size_t size, size_t nmemb, void *userp) // libcurl -
{
    QStringList* fileList = static_cast<QStringList*>(userp);
    QString data = QString::fromUtf8(static_cast<char*>(contents), size * nmemb);

    fileList->append(parseFTPList(data));

    return size * nmemb;
}

QStringList Network::parseFTPList(const QString &response) { //Output-Datei filtern mit Regex
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

size_t Network::WriteCallBack(void *contents, size_t size, size_t nmemb, void *userp) {
    std::ofstream* file = static_cast<std::ofstream*>(userp);
    file->write(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

void Network::refreshServer() // Server-Dateien Aktualisieren/Fetchen
{
    // FTP-Dateien abrufen
    QString ftpUrl = "ftp://138.199.195.70:21/files/";
    QString username = "bob";
    QString password = "Kartoffel123?!";
    QStringList files = Network::getFTPFileList(ftpUrl, username, password);

    m_dataFileFromFtpServer.clear();
    for (const QString& file : files) {
        m_dataFileFromFtpServer.append(file);
    }
    qDebug() << m_dataFileFromFtpServer;
    emit dataFileFromFtpServerChanged();
}

void Network::ThemeDatabaseSelected(const QString &db)
{
    m_nameDatabaseSelected = db;
    qDebug() << m_nameDatabaseSelected << " qml file";
    emit nameDatabaseSelectedChanged();
}

void Network::downloadFile() //Download
{
    CURL *curl;
    CURLcode res;

    QString ftpUrl = "ftp://138.199.195.70:21/files/";

    if(m_nameDatabaseSelected != "") {
        QString downloadDir = pathSystem + "/data_Lernapp/datenbank_Lernapp";
        ftpUrl += m_nameDatabaseSelected;

        // Dateinamen aus der URL extrahieren
        QString fileName = QUrl(ftpUrl).fileName();
        QString filePath = downloadDir + "/" + fileName;
        std::string filePathStd = filePath.toStdString();

        std::ofstream file(filePathStd, std::ios::binary);
        if (!file) {
            qDebug() << "Fehler: Datei konnte nicht erstellt werden!";
        } else {
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.toStdString().c_str());
                curl_easy_setopt(curl, CURLOPT_USERPWD, "bob:Kartoffel123?!");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Network::WriteCallBack);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                if (res != CURLE_OK) {
                    qDebug() << "curl error:" << curl_easy_strerror(res);
                }
                else {
                    qDebug() << "Download erfolgreich! Datei gespeichert unter:" << filePath;
                }

            }
        }
    } else {
        qDebug() << "Datei nicht gefunden";
    }
    curl_global_cleanup();
    // refreshServer();
}

QStringList Network::dataFileFromFtpServer() const //dataFileFromFtpServer
{
    return m_dataFileFromFtpServer;
}

void Network::setDataFileFromFtpServer(const QStringList &newDataFileFromFtpServer)
{
    if (m_dataFileFromFtpServer == newDataFileFromFtpServer)
        return;
    m_dataFileFromFtpServer = newDataFileFromFtpServer;
    emit dataFileFromFtpServerChanged();
}

QString Network::nameDatabaseSelected() const //nameDatabaseSelected
{
    return m_nameDatabaseSelected;
}

void Network::setNameDatabaseSelected(const QString &newNameDatabaseSelected)
{
    if (m_nameDatabaseSelected == newNameDatabaseSelected)
        return;
    m_nameDatabaseSelected = newNameDatabaseSelected;
    emit nameDatabaseSelectedChanged();
}
