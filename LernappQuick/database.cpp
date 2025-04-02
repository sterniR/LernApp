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

void Database::getFileName(const QString &fileName) // Ausgewählte Datenbank auswählen
{
    setSelectedLocalFileName(fileName);
    qDebug() << (m_selectedLocalFileName);
    emit selectedLocalFileNameChanged();
}

int Database::getNumberOfQuestions(const QString &databaseName) // Anzahl der Fragen bekommen
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

void Database::fillQuestionList() // Alle Fragen füllen
{
    QSqlQuery query(database);
    query.prepare("SELECT frage_text FROM Fragen ORDER BY id;");

    if(!query.exec()) {
        error_query(query.lastError());
    } else {
        m_questionList.clear();
        while(query.next()) {
            m_questionList.append(query.value(0).toString());
            emit questionListChanged();
        }
    }
    query.clear();
    qDebug() << m_questionList;
}

void Database::fillTrueFalseList()
{
    QSqlQuery query(database);
    query.prepare("SELECT ist_wahr FROM Fragen ORDER BY id;");

    if(!query.exec()) {
        error_query(query.lastError());
    } else {
        m_optionListFromTrueFalse.clear();
        while(query.next()) {
            m_optionListFromTrueFalse.append(query.value(0).toString());
            emit optionListFromTrueFalseChanged();
        }
    }
    query.clear();
    qDebug() << m_optionListFromTrueFalse << " Richtig / Ist Wahr";
}

void Database::fillInputList()
{
    QSqlQuery query(database);
    query.prepare("SELECT antwort_mit_eingabe FROM Fragen ORDER BY id;");

    if(!query.exec()) {
        error_query(query.lastError());
    } else {
        m_optionListFromInput.clear();
        while(query.next()) {
            m_optionListFromInput.append(query.value(0).toString());
            emit optionListFromInputChanged();
        }
    }
    query.clear();
    qDebug() << m_optionListFromInput << " Input Eingabe";
}

void Database::fillStatusList() // Alle status/arten von Fragen füllen
{
    QSqlQuery query(database);
    query.prepare("SELECT status FROM Fragen;");

    if(!query.exec()) {
        error_query(query.lastError());
    } else {
        m_statusList.clear();
        while(query.next()) {
            m_statusList.append(query.value(0).toString());
            emit statusListChanged();
        }
    }
    qDebug() << m_statusList << " Status Liste";
}

void Database::fillOptionList() // Multiple Choise Antworten füllen
{
    QSqlQuery query(database);
    query.prepare("SELECT ist_wahr_mit_text, ist_falsch_mit_text, ist_falsch2_mit_text FROM Fragen;");

    if(!query.exec()) {
        error_query(query.lastError());
    } else {
        m_optionListFromMultiQuestion.clear();
        while(query.next()) {
            QStringList options;
            options << query.value(0).toString()
                    << query.value(1).toString()
                    << query.value(2).toString();

            m_optionListFromMultiQuestion.append(options);
            emit optionListFromMultiQuestionChanged();
        }
    }
    qDebug() << m_optionListFromMultiQuestion << " 3x Multiple Choise";
}

void Database::nextWord() // Fragen Zähler erhöhen
{
    if(m_questionList.size() > m_currentIndex) {
        m_currentIndex++;
        emit currentIndexChanged();
    }
    showQuestion();
}

QString Database::showQuestion() // Frage aktualisieren
{

    if(m_questionList.size() > m_counterQuestion) {
        m_question = m_questionList.at(m_counterQuestion);
        emit questionChanged();
        m_counterQuestion++;
        emit counterQuestionChanged();
    }
    return m_question;
}

void Database::checkAnswerTrueFalse(const QString &selected)
{
    m_userAnswerTrueFalse = selected;
    qDebug() << m_userAnswerTrueFalse;
    if(m_userAnswerTrueFalse == m_optionListFromTrueFalse.at(counter)) {
        qDebug() << "Richtig";
        counter++;
        m_correctPoints = counter;
        emit correctPointsChanged();
        qDebug() << counter << " :counter";
        qDebug() << m_correctPoints << " :Points";
    } else {
        qDebug() << "Falsch";
        counter++;
        qDebug() << counter << " :counter";
        qDebug() << m_correctPoints << " :Points";
    }
}

void Database::checkAnswerMultiChoise(const QString &selected)
{
    m_userAnswerMultiChoise = selected;
    qDebug() << m_userAnswerMultiChoise;

    QStringList list = m_optionListFromMultiQuestion.at(counter);
    if(m_userAnswerMultiChoise == list.at(0)) {
        qDebug() << "Richtig";
        counter++;
        m_correctPoints = counter;
        emit correctPointsChanged();
        qDebug() << counter << " :counter";
        qDebug() << m_correctPoints << " :Points";
    } else {
        qDebug() << "Falsch";
        counter++;
        qDebug() << counter << " :counter";
        qDebug() << m_correctPoints << " :Points";
    }
}

void Database::checkAnswerInput(const QString &selected)
{
    m_userAnswerInput = selected;
    qDebug() << m_userAnswerInput;
    if(m_userAnswerInput == m_optionListFromInput.at(counter)) {
        qDebug() << "Richtig";
        counter++;
        m_correctPoints = counter;
        emit correctPointsChanged();
        qDebug() << counter << " :counter";
        qDebug() << m_correctPoints << " :Points";
    } else {
        qDebug() << "Falsch";
        counter++;
        qDebug() << counter << " :counter";
        qDebug() << m_correctPoints << " :Points";
    }
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


QStringList Database::statusList() const
{
    return m_statusList;
}

void Database::setStatusList(const QStringList &newStatusList)
{
    if (m_statusList == newStatusList)
        return;
    m_statusList = newStatusList;
    emit statusListChanged();
}


QList<QStringList> Database::optionListFromMultiQuestion() const
{
    return m_optionListFromMultiQuestion;
}

void Database::setOptionListFromMultiQuestion(const QList<QStringList> &newOptionListFromMultiQuestion)
{
    if (m_optionListFromMultiQuestion == newOptionListFromMultiQuestion)
        return;
    m_optionListFromMultiQuestion = newOptionListFromMultiQuestion;
    emit optionListFromMultiQuestionChanged();
}


QString Database::userAnswerMultiChoise() const
{
    return m_userAnswerMultiChoise;
}

void Database::setUserAnswerMultiChoise(const QString &newUserAnswerMultiChoise)
{
    if (m_userAnswerMultiChoise == newUserAnswerMultiChoise)
        return;
    m_userAnswerMultiChoise = newUserAnswerMultiChoise;
    emit userAnswerMultiChoiseChanged();
}


QString Database::userAnswerTrueFalse() const
{
    return m_userAnswerTrueFalse;
}

void Database::setUserAnswerTrueFalse(const QString &newUserAnswerTrueFalse)
{
    if (m_userAnswerTrueFalse == newUserAnswerTrueFalse)
        return;
    m_userAnswerTrueFalse = newUserAnswerTrueFalse;
    emit userAnswerTrueFalseChanged();
}


QString Database::userAnswerInput() const
{
    return m_userAnswerInput;
}

void Database::setUserAnswerInput(const QString &newUserAnswerInput)
{
    if (m_userAnswerInput == newUserAnswerInput)
        return;
    m_userAnswerInput = newUserAnswerInput;
    emit userAnswerInputChanged();
}


int Database::correctPoints() const
{
    return m_correctPoints;
}

void Database::setCorrectPoints(int newCorrectPoints)
{
    counter = 0;
    if (m_correctPoints == newCorrectPoints)
        return;
    m_correctPoints = newCorrectPoints;
    emit correctPointsChanged();
}


QStringList Database::optionListFromTrueFalse() const
{
    return m_optionListFromTrueFalse;
}

void Database::setOptionListFromTrueFalse(const QStringList &newOptionListFromTrueFalse)
{
    if (m_optionListFromTrueFalse == newOptionListFromTrueFalse)
        return;
    m_optionListFromTrueFalse = newOptionListFromTrueFalse;
    emit optionListFromTrueFalseChanged();
}


QStringList Database::optionListFromInput() const
{
    return m_optionListFromInput;
}

void Database::setOptionListFromInput(const QStringList &newOptionListFromInput)
{
    if (m_optionListFromInput == newOptionListFromInput)
        return;
    m_optionListFromInput = newOptionListFromInput;
    emit optionListFromInputChanged();
}
