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

QStringList Database::listLocalDatabase() //Lokalen Verzeichnis mit Datein vom der Datenbank anzeigen
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
        return fileList;
    }
}
