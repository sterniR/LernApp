#include "lernapp.h"
#include "./ui_lernapp.h"
#include "curl/curl.h"

#include <fstream>
#include <QApplication>
#include <QUrl>
#include <iostream>

Lernapp::Lernapp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lernapp)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    // QVBoxLayout* layoutTree = new QVBoxLayout(this);

    // Datenbank ertellen
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("Datenbank.sqlite3");
    if(!database.open())
    {
        error_database(database);
    }

    QSqlQuery query(database);
    query.prepare("CREATE TABLE IF NOT EXISTS Fragen ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "frage_text TEXT UNIQUE NOT NULL,"
                  "ist_wahr BOOLEAN NOT NULL DEFAULT 0,"
                  "erstellt_von TEXT,"
                  "erstellt_am TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                  ");");
    if(!query.exec())
        error_query(query.lastError());

    // query.prepare("INSERT OR IGNORE INTO Fragen ("
    //               "frage_text, ist_wahr, erstellt_von)"
    //               "VALUES ('Ist die Erde Rund?', '1', 'Roman'"
    //               ");");
    // if(!query.exec())
    //     error_query(query.lastError());
}

Lernapp::~Lernapp()
{
    delete ui;
}

void Lernapp::connect()
{

}

//Seite 0

void Lernapp::on_button1_1_clicked()
{
    createDataEntry();
    ui->stackedWidget->setCurrentIndex(1);
}
void Lernapp::on_button1_2_clicked()
{
    createDataEntry();
    ui->stackedWidget->setCurrentIndex(2);
}
void Lernapp::on_button1_3_clicked() // Datenbank öffnen
{
    ui->stackedWidget->setCurrentIndex(3);

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabels(QStringList() << "FTP-Dateien");
}

//Seite 1

void Lernapp::on_button2_1_clicked() // Liste öffnen
{
    createDataEntry();
    ui->stackedWidget->setCurrentIndex(2);
}
void Lernapp::on_button2_2_clicked() //Startseite
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Lernapp::on_button2_3_clicked() // Frage speichern
{
    frageText = ui->textEdit2_1->toPlainText();
    erstelltVonText = ui->lineEdit2_1->displayText();

    if(ui->textEdit2_1->toPlainText() == "")
    {
        QMessageBox::warning(this,"Kein Text","Bitte geben Sie eine Frage ein.");
    }
    else if(frageTextKontrolle == frageText)
    {
        QMessageBox::warning(this,"Gleiche Frage","Diese Frage existiert schon.\nBitte aendern Sie die Frage.");
    }
    else if(ui->radiobutton2_1->isChecked())
    {
        istWahrText = "1";
    }
    else if(ui->radiobutton2_2->isChecked())
    {
        istWahrText = "0";
    }
    else
    {
        QMessageBox::warning(this,"Kein Wahrheitswert ausgeweahlt", "Bitte 'Wahr' oder 'Falsch' auswaehlen.");
    }


    if(ui->textEdit2_1->toPlainText() == "")
    {

    }
    else if((!ui->radiobutton2_1->isChecked() && !ui->radiobutton2_2->isChecked()))
    {

    }
    else if(frageTextKontrolle == frageText)
    {

    }
    else
    {
        QSqlQuery query(database);
        query.prepare("INSERT OR IGNORE INTO Fragen ("
                      "frage_text, ist_wahr, erstellt_von)"
                      "VALUES ('" +
                      frageText + "', '" + istWahrText + "', '" + erstelltVonText +
                      "');");
        if(!query.exec())
            error_query(query.lastError());
        createDataEntry();
    }
}

//Seite 2

void Lernapp::on_button3_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void Lernapp::on_button3_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//Seite 3

void Lernapp::on_button4_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Lernapp::on_button4_4_clicked()
{
    CURL *curl;
    CURLcode res;

    QString ftpUrl = "ftp://138.199.195.70:21/files/test.txt";
    // Projektverzeichnis ermitteln
    QString projectDir = QCoreApplication::applicationDirPath();

    // Download-Ordner setzen
    QString downloadDir = projectDir + "/download";

    // Sicherstellen, dass der "download"-Ordner existiert
    QDir().mkpath(downloadDir);

    // Dateinamen aus der URL extrahieren
    QString fileName = QUrl(ftpUrl).fileName();
    QString filePath = downloadDir + "/" + fileName;
    std::string filePathStd = filePath.toStdString();

    // Datei im "download"-Ordner speichern
    std::ofstream file(filePathStd, std::ios::binary);
    if (!file) {
        qDebug() << "Fehler: Datei konnte nicht erstellt werden!";
    }

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, "bob:Kartoffel123?!");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Lernapp::WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK)
            qDebug() << "curl error:" << curl_easy_strerror(res);
        else
            qDebug() << "Download erfolgreich! Datei gespeichert unter:" << filePath;

    }
    curl_global_cleanup();
}

void Lernapp::on_button4_5_clicked() {
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabels(QStringList() << "FTP-Dateien");

    // FTP-Dateien abrufen
    QString ftpUrl = "ftp://138.199.195.70:21/files/";
    QString username = "bob";  // Falls anonym, einfach leer lassen
    QString password = "Kartoffel123?!";  // Falls anonym, einfach leer lassen
    QStringList files = getFTPFileList(ftpUrl, username, password);

    // Dateien zum TreeWidget hinzufügen
    for (const QString& file : files) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, file);
        ui->treeWidget->addTopLevelItem(item);
    }

    // layoutTree->addWidget(ui->treeWidget);
    // setLayout(layoutTree);
}
// Debugging

void Lernapp::on_actionNext_triggered()
{
    ui->stackedWidget->setCurrentIndex((ui->stackedWidget->currentIndex()+1));
    QString site = "Seite: " + QString::number(ui->stackedWidget->currentIndex());
    ui->menuSeite->setTitle(site);
}
void Lernapp::on_actionBack_triggered()
{
    ui->stackedWidget->setCurrentIndex((ui->stackedWidget->currentIndex()-1));
    QString site = "Seite: " + QString::number(ui->stackedWidget->currentIndex());
    ui->menuSeite->setTitle(site);
}

//Database debugging

void Lernapp::error_database(QSqlDatabase error)
{
    QMessageBox::warning(nullptr, tr("Fehler in der Datenbank"),tr("Fehler: %1").arg(error.lastError().text()));
}
void Lernapp::error_query(QSqlError error)
{
    QMessageBox::warning(nullptr, tr("Fehler in der Query"),tr("Fehler: %1").arg(error.text()));

}

//Funktionen
void Lernapp::createDataEntry()
{
    frageTextKontrolle = ui->textEdit2_1->toPlainText();

    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("Fragen");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Frage"));
    model->setHeaderData(2, Qt::Horizontal, tr("Wahr/Falsch"));
    model->setHeaderData(3, Qt::Horizontal, tr("Erstellt von"));
    model->setHeaderData(4, Qt::Horizontal, tr("Erstellt am"));

    ui->tableView2_1->setModel(model);
    ui->tableView2_1->hideColumn(0);
    ui->tableView2_1->show();
}

//cUrl Funktionen
size_t Lernapp::WriteCallBack(void *contents, size_t size, size_t nmemb, void *userp)
{
    std::ofstream* file = static_cast<std::ofstream*>(userp);
    file->write(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

size_t Lernapp::getDirFtp(void* contents, size_t size, size_t nmemb, void* userp) {
    QStringList* fileList = static_cast<QStringList*>(userp);
    QString data = QString::fromUtf8(static_cast<char*>(contents), size * nmemb);

    // Jede Zeile enthält eine Datei
    fileList->append(data.split("\n", Qt::SkipEmptyParts));

    return size * nmemb;
}

QStringList Lernapp::getFTPFileList(const QString& ftpUrl, const QString& username, const QString& password) {
    QStringList fileList;
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.toStdString().c_str());

        // Falls Login notwendig ist
        QString userPwd = username + ":" + password;
        curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.toStdString().c_str());

        // FTP Listing abrufen
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Lernapp::getDirFtp);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileList);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl Fehler: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return fileList;
}



