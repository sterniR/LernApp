#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    m_counterQuestion = 0;
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

int Database::getNumberOfQuestions(const QString &databaseName)
{
    if(database.isOpen()) {
        database.close();
    }
    database.setDatabaseName(pathSystem + "/data_Lernapp/datenbank_Lernapp/" + databaseName);
    if(!database.open())
    {
        error_database(database);
    }

    QSqlQuery query(database);
    int sum = -1;
    query.prepare("SELECT COUNT(*) FROM Fragen;");

    if(!query.exec()) {
        error_query(query.lastError());
        return sum;
    }

    while(query.next()) {
        sum = query.value(0).toInt();
        return sum;
    }

    return sum;
}

//Error Message Sql Query
void Database::error_database(QSqlDatabase error)
{
    qDebug() << "Fehler in der Datenbank: " << error.lastError().text();
}
void Database::error_query(QSqlError error)
{
    qDebug() << "Fehler in der Query: " << error.text();
}

void Database::fillQuestionList()
{
    QSqlQuery query(database);
    query.prepare("SELECT frage_text FROM Fragen;");

    if(!query.exec()) {
        error_query(query.lastError());
    } else {
        m_questionList.clear();
        while(query.next()) {
            m_questionList.append(query.value(0).toString());
        }
    }
    qDebug() << m_questionList;
}

void Database::nextWord()
{
    if(m_questionList.size() > m_currentIndex) {
        m_currentIndex++;
        emit currentIndexChanged();
    }
    showQuestion();
}

QString Database::showQuestion()
{
    // if(m_questionList.size() > counter) {
    // }
    if(m_questionList.size() > m_counterQuestion) {
        m_question = m_questionList.at(m_counterQuestion);
        emit questionChanged();
        m_counterQuestion++;
        emit counterQuestionChanged();
    }
    return m_question;
}

//Properties
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


const QStringList &Database::questionList() const
{
    return m_questionList;
}

void Database::setQuestionList(const QStringList &newQuestionList)
{
    if (m_questionList == newQuestionList)
        return;
    m_questionList = newQuestionList;
    emit questionListChanged();
}


int Database::currentIndex() const
{
    return m_currentIndex;
}

void Database::setCurrentIndex(int newCurrentIndex)
{
    if (m_currentIndex == newCurrentIndex)
        return;
    m_currentIndex = newCurrentIndex;
    emit currentIndexChanged();
}


QString Database::question() const
{
    return m_question;
}

void Database::setQuestion(const QString &newQuestion)
{
    if (m_question == newQuestion)
        return;
    m_question = newQuestion;
    emit questionChanged();
}


int Database::counterQuestion() const
{
    return m_counterQuestion;
}

void Database::setCounterQuestion(int newCounterQuestion)
{
    if (m_counterQuestion == newCounterQuestion)
        return;
    m_counterQuestion = newCounterQuestion;
    emit counterQuestionChanged();
}
