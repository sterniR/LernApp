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
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

class Database : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QStringList& listLocalDir READ listLocalDir WRITE setListLocalDir NOTIFY listLocalDirChanged FINAL)
    Q_PROPERTY(const QString& selectedLocalFileName READ selectedLocalFileName WRITE setSelectedLocalFileName NOTIFY selectedLocalFileNameChanged FINAL)
    QML_ELEMENT
public:
    explicit Database(QObject *parent = nullptr);

    QSqlDatabase database;
    QString const pathSystem = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);


    Q_INVOKABLE QString setupDatabaseDir();
    Q_INVOKABLE QStringList showLocalDatabase();
    Q_INVOKABLE void deleteLocalFile(QString selectedLocalFileName);
    Q_INVOKABLE void getFileName(const QString& fileName);
    Q_INVOKABLE QString getNumberOfQuestions(const QString& databaseName);

    const QStringList &listLocalDir() const;
    void setListLocalDir(const QStringList &newListLocalDir);

    const QString &selectedLocalFileName() const;
    void setSelectedLocalFileName(const QString &newSelectedLocalFileName);

public slots:

signals:
    void listLocalDirChanged();
    void selectedLocalFileNameChanged();

private:
    QStringList m_listLocalDir;
    QString m_selectedLocalFileName;

private slots:
    void error_database(QSqlDatabase);
    void error_query(QSqlError);
};

#endif // DATABASE_H
