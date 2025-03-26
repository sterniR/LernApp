#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
    database = QSqlDatabase::addDatabase("QSQLITE");
}

QString Database::setupDatabaseDir() //Erstellte lokale Ordner für die Datenbank
{
    QString pathDatabase;
    pathDatabase = pathSystem + "/data_Lernapp/datenbank_Lernapp";

    QDir dir;
    if (!dir.exists(pathDatabase)) {
        if (!dir.mkpath(pathDatabase)) {
            qWarning() << "Fehler beim Erstellen des Ordners:" << pathDatabase;
            return pathDatabase;
        }
    }
    qDebug() << pathDatabase;
    return pathDatabase;
}

QStringList Database::showLocalDatabase() //Lokalen Verzeichnis mit Datein vom der Datenbank anzeigen
{
    QString pathLocaleFiles = pathSystem + "/data_Lernapp/datenbank_Lernapp";
    QDir directory(pathLocaleFiles);
    QStringList fileList = directory.entryList(QDir::Files);

    if (!directory.exists()) {
        qDebug() << "Ordner existiert nicht:" << pathLocaleFiles;
        return fileList;
    } else {
        QFileSystemModel model;
        model.setRootPath(pathLocaleFiles);
        QModelIndex rootIndex = model.index(pathLocaleFiles);

        int rowCount = model.rowCount(rootIndex);

        for(int row = 0; row < rowCount; ++row) {
            QModelIndex index = model.index(row, 0 ,rootIndex);
            QString fileName = model.fileName(index);
            fileList.append(fileName);
        }
        qDebug() << fileList;
        m_listLocalDir.clear();
        setListLocalDir(fileList);
        emit listLocalDirChanged();
        return fileList;
    }
}

void Database::deleteLocalFile(QString selectedLocalFileName) // Lokale Datenbank/File löschen
{
    QFile db;
    if(selectedLocalFileName != "") {
        if(QDir::setCurrent(pathSystem + "/data_Lernapp/datenbank_Lernapp")) {
            db.setFileName(selectedLocalFileName);
            db.remove();
            selectedLocalFileName.clear();
        } else {
            qDebug() << "Datei konnte nicht gelöscht werden";
        }
    }
}

void Database::getFileName(const QString &fileName)
{
    setSelectedLocalFileName(fileName);
    qDebug() << (m_selectedLocalFileName);
    emit selectedLocalFileNameChanged();
}

const QStringList &Database::listLocalDir() const
{
    return m_listLocalDir;
}

void Database::setListLocalDir(const QStringList &newListLocalDir)
{
    if (m_listLocalDir == newListLocalDir)
        return;
    m_listLocalDir = newListLocalDir;
    emit listLocalDirChanged();
}

const QString &Database::selectedLocalFileName() const
{
    return m_selectedLocalFileName;
}

void Database::setSelectedLocalFileName(const QString &newSelectedLocalFileName)
{
    if (m_selectedLocalFileName == newSelectedLocalFileName)
        return;
    m_selectedLocalFileName = newSelectedLocalFileName;
    emit selectedLocalFileNameChanged();
}
