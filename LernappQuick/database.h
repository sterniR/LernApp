#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QQmlEngine>

#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QFileSystemModel>
#include <QRegularExpression>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Database : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QStringList& listLocalDir READ listLocalDir WRITE setListLocalDir NOTIFY listLocalDirChanged FINAL)
    QML_ELEMENT
public:
    explicit Database(QObject *parent = nullptr);

    QSqlDatabase database;
    QString const pathSystem = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    Q_INVOKABLE QString setupDatabaseDir();
    Q_INVOKABLE QStringList listLocalDatabase();

    const QStringList &listLocalDir() const;
    void setListLocalDir(const QStringList &newListLocalDir);

public slots:

signals:
    void listLocalDirChanged();
private:
    QStringList m_listLocalDir;
};

#endif // DATABASE_H
