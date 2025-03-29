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
    Q_PROPERTY(const QStringList& questionList READ questionList WRITE setQuestionList NOTIFY questionListChanged FINAL)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged FINAL)
    Q_PROPERTY(QString question READ question WRITE setQuestion NOTIFY questionChanged FINAL)
    Q_PROPERTY(int counterQuestion READ counterQuestion WRITE setCounterQuestion NOTIFY counterQuestionChanged FINAL)
    QML_ELEMENT
public:
    explicit Database(QObject *parent = nullptr);

    Q_INVOKABLE QString setupDatabaseDir();
    Q_INVOKABLE QStringList showLocalDatabase();
    Q_INVOKABLE void deleteLocalFile(QString selectedLocalFileName);
    Q_INVOKABLE void getFileName(const QString& fileName);
    Q_INVOKABLE int getNumberOfQuestions(const QString& databaseName);
    Q_INVOKABLE void fillQuestionList();
    Q_INVOKABLE void nextWord();
    Q_INVOKABLE QString showQuestion();

    const QStringList &listLocalDir() const;
    void setListLocalDir(const QStringList &newListLocalDir);

    const QString &selectedLocalFileName() const;
    void setSelectedLocalFileName(const QString &newSelectedLocalFileName);

    const QStringList &questionList() const;
    void setQuestionList(const QStringList &newQuestionList);

    int currentIndex() const;
    Q_INVOKABLE void setCurrentIndex(int newCurrentIndex);

    QString question() const;
    void setQuestion(const QString &newQuestion);

    int counterQuestion() const;
    void setCounterQuestion(int newCounterQuestion);

public slots:

signals:
    void listLocalDirChanged();
    void selectedLocalFileNameChanged();
    void questionListChanged();

    void currentIndexChanged();

    void questionChanged();

    void counterQuestionChanged();

private:
    QSqlDatabase database;
    QString const pathSystem = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    int counter = 0;

    QStringList m_listLocalDir;
    QString m_selectedLocalFileName;
    QStringList m_questionList;

    int m_currentIndex;

    QString m_question;

    int m_counterQuestion;

private slots:
    void error_database(QSqlDatabase);
    void error_query(QSqlError);
};

#endif // DATABASE_H
