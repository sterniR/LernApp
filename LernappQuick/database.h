#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QQmlEngine>

#include <QDir>
#include <QList>
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
    Q_PROPERTY(QStringList statusList READ statusList WRITE setStatusList NOTIFY statusListChanged FINAL)

    Q_PROPERTY(QList<QStringList> optionListFromMultiQuestion READ optionListFromMultiQuestion WRITE setOptionListFromMultiQuestion NOTIFY optionListFromMultiQuestionChanged FINAL)
    Q_PROPERTY(QStringList optionListFromTrueFalse READ optionListFromTrueFalse WRITE setOptionListFromTrueFalse NOTIFY optionListFromTrueFalseChanged FINAL)
    Q_PROPERTY(QStringList optionListFromInput READ optionListFromInput WRITE setOptionListFromInput NOTIFY optionListFromInputChanged FINAL)

    Q_PROPERTY(QString userAnswerMultiChoise READ userAnswerMultiChoise WRITE setUserAnswerMultiChoise NOTIFY userAnswerMultiChoiseChanged FINAL)
    Q_PROPERTY(QString userAnswerTrueFalse READ userAnswerTrueFalse WRITE setUserAnswerTrueFalse NOTIFY userAnswerTrueFalseChanged FINAL)
    Q_PROPERTY(QString userAnswerInput READ userAnswerInput WRITE setUserAnswerInput NOTIFY userAnswerInputChanged FINAL)

    Q_PROPERTY(int correctPoints READ correctPoints WRITE setCorrectPoints NOTIFY correctPointsChanged FINAL)
    QML_ELEMENT
public:
    explicit Database(QObject *parent = nullptr);

    Q_INVOKABLE QString setupDatabaseDir();
    Q_INVOKABLE QStringList showLocalDatabase();
    Q_INVOKABLE void deleteLocalFile(QString selectedLocalFileName);
    Q_INVOKABLE void getFileName(const QString& fileName);
    Q_INVOKABLE int getNumberOfQuestions(const QString& databaseName);

    Q_INVOKABLE void fillQuestionList();    // Fragen
    Q_INVOKABLE void fillTrueFalseList();   // Richtig oder Falsch
    Q_INVOKABLE void fillInputList();       // Wort Input
    Q_INVOKABLE void fillOptionList();      // Multi
    Q_INVOKABLE void fillStatusList();      // Status

    Q_INVOKABLE void nextWord();
    Q_INVOKABLE QString showQuestion();

    Q_INVOKABLE void checkAnswerTrueFalse(const QString& selected);
    Q_INVOKABLE void checkAnswerMultiChoise(const QString& selected);
    Q_INVOKABLE void checkAnswerInput(const QString& selected);

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

    QStringList statusList() const;
    void setStatusList(const QStringList &newStatusList);

    QList<QStringList> optionListFromMultiQuestion() const;
    void setOptionListFromMultiQuestion(const QList<QStringList> &newOptionListFromMultiQuestion);

    QString userAnswerMultiChoise() const;
    void setUserAnswerMultiChoise(const QString &newUserAnswerMultiChoise);

    QString userAnswerTrueFalse() const;
    void setUserAnswerTrueFalse(const QString &newUserAnswerTrueFalse);

    QString userAnswerInput() const;
    void setUserAnswerInput(const QString &newUserAnswerInput);

    int correctPoints() const;
    Q_INVOKABLE void setCorrectPoints(int newCorrectPoints);

    QStringList optionListFromTrueFalse() const;
    void setOptionListFromTrueFalse(const QStringList &newOptionListFromTrueFalse);

    QStringList optionListFromInput() const;
    void setOptionListFromInput(const QStringList &newOptionListFromInput);

public slots:

signals:
    void listLocalDirChanged();
    void selectedLocalFileNameChanged();
    void questionListChanged();

    void currentIndexChanged();

    void questionChanged();

    void counterQuestionChanged();

    void statusListChanged();

    void question3XQuestionOptionChanged();

    void optionListFromMultiQuestionChanged();

    void userAnswerMultiChoiseChanged();

    void userAnswerTrueFalseChanged();

    void userAnswerInputChanged();

    void correctPointsChanged();

    void optionListFromTrueFalseChanged();

    void optionListFromInputChanged();

private:
    QSqlDatabase database;
    QString const pathSystem = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    int counter = 0;

    QStringList m_listLocalDir;
    QString m_selectedLocalFileName;
    QStringList m_questionList;

    int m_currentIndex = 0;

    QString m_question;

    int m_counterQuestion = 0;

    QStringList m_statusList;


    QList<QStringList> m_optionListFromMultiQuestion;

    QString m_userAnswerMultiChoise;

    QString m_userAnswerTrueFalse;

    QString m_userAnswerInput;

    int m_correctPoints = 0;

    QStringList m_optionListFromTrueFalse;

    QStringList m_optionListFromInput;

private slots:
    void error_database(QSqlDatabase);
    void error_query(QSqlError);
};

#endif // DATABASE_H
