#include "lernapp.h"
#include "./ui_lernapp.h"
#include "curl/curl.h"

#include <fstream>
#include <iostream>

Lernapp::Lernapp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lernapp)
{
    ui->setupUi(this);
    disableTabs();
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->treeView4_1, &QTreeView::clicked, this, &::Lernapp::treeView4ItemClicked);
    connect(ui->treeView5_1, &QTreeView::clicked, this, &::Lernapp::treeView5ItemClicked);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &Lernapp::deleteDataEntryServer);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &Lernapp::lastSelectedTab);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &Lernapp::clickedServerTab);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &Lernapp::clickedUebersichtTab);

    setupDir();
    setupDatabaseDir();
    database = QSqlDatabase::addDatabase("QSQLITE");

    // QSqlQuery query(database);
    // query.prepare("CREATE TABLE IF NOT EXISTS Fragen ("
    //               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    //               "frage_text TEXT UNIQUE NOT NULL,"
    //               "ist_wahr BOOLEAN NOT NULL DEFAULT 0,"
    //               "erstellt_von TEXT,"
    //               "erstellt_am TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
    //               ");");
    // if(!query.exec())
    //     error_query(query.lastError());

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

//Seite 1

void Lernapp::on_button_editorStart_clicked() // Editor starten
{
    ui->tabWidget->setTabVisible(4,true);

    ui->tabWidget->setTabEnabled(1,false);
    ui->tabWidget->setTabVisible(1,true);

    ui->tabWidget->setTabEnabled(2,false);
    ui->tabWidget->setTabVisible(2,true);

    // ui->tabWidget->setTabEnabled(3,false);
    ui->tabWidget->setTabVisible(3,true);


    listDatabaseTreeView(ui->treeView5_1);
    isDatabaseActive();

    ui->tabWidget->setCurrentIndex(4);
}

//Seite 2

void Lernapp::on_button2_2_clicked() //Startseite
{
    disableTabs();
    ui->tabWidget->setCurrentIndex(0);
}
void Lernapp::on_button2_3_clicked() // Frage speichern
{
    frageText = ui->textEdit2_1->toPlainText();
    erstelltVonText = ui->lineEdit_createNewDatabase->displayText();

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
        listDatabaseTableView(ui->tableView2_1);
    }
}

void Lernapp::on_button2_4_clicked() //Datenbank umbenennen
{
    if(activeDatabase != "") {

        QFile file;
        QDir::setCurrent(pathSystem + "/data_Lernapp/datenbank_Lernapp/");
        file.setFileName(activeDatabase);

        if(!file.rename(ui->lineEdit2_3->text())) {
            QMessageBox::warning(this, "Fehler", tr("Datenbank konnte nicht umbenannt werden.\n%1").arg(file.errorString()));
            qDebug() << file.fileName();
        } else {
            ui->label2_1->setText(tr("Aktive Datenbank: %1").arg(ui->lineEdit2_3->text()));
            listDatabaseTableView(ui->tableView2_1);
        }
    } else {
        QMessageBox::warning(this, "Fehler", "Bitte wählen Sie eine Datenbank vorher aus.");
    }
    ui->lineEdit2_3->clear();
}

void Lernapp::on_button_createNewDatabase_clicked() //Neue Datenbank erstellen
{
    activeDatabase = ui->lineEdit_createNewDatabase->text();
    selectDatabase(activeDatabase);
    ui->label2_1->setText(tr("Aktive Datenbank: %1").arg(activeDatabase));
    listDatabaseTableView(ui->tableView2_1);
    ui->lineEdit_createNewDatabase->clear();
}

//Seite 3

void Lernapp::on_button3_2_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

//Seite 4

void Lernapp::on_button4_2_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void Lernapp::on_button4_4_clicked() //Download
{
    CURL *curl;
    CURLcode res;

    QString ftpUrl = "ftp://138.199.195.70:21/files/";

    if(ui->treeWidget->currentItem() != NULL) {
        QTreeWidgetItem *selectedFile = ui->treeWidget->currentItem();
        QString itemText = selectedFile->text(ui->treeWidget->currentColumn());

        ftpUrl += itemText;

        // Download-Ordner setzen
        QString downloadDir = pathSystem + "/data_Lernapp/datenbank_Lernapp";

        // Sicherstellen, dass der "download"-Ordner existiert
        // QDir().mkpath(downloadDir);

        // Dateinamen aus der URL extrahieren
        QString fileName = QUrl(ftpUrl).fileName();
        QString filePath = downloadDir + "/" + fileName;
        std::string filePathStd = filePath.toStdString();

        // Datei im "download"-Ordner speichern
        std::ofstream file(filePathStd, std::ios::binary);
        if (!file) {
            qDebug() << "Fehler: Datei konnte nicht erstellt werden!";
            QMessageBox::information(nullptr, tr("Fehler"), tr("Datei konnte nicht erstellt werden!"));
        } else {
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.toStdString().c_str());
                curl_easy_setopt(curl, CURLOPT_USERPWD, "bob:Kartoffel123?!");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Lernapp::WriteCallBack);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                if (res != CURLE_OK)
                    QMessageBox::warning(nullptr, tr("Server Error"), tr(curl_easy_strerror(res)));
                // qDebug() << "curl error:" << curl_easy_strerror(res);
                else
                    QMessageBox::information(nullptr, tr("Download erfolgreich"), tr("Datei gepeichert unter %1").arg(filePath));
                // qDebug() << "Download erfolgreich! Datei gespeichert unter:" << filePath;

            }
        }
    } else {
        QMessageBox::warning(nullptr, tr("Datei nicht gefunden"), tr("Bitten waehlen Sie eine Datei aus der unteren Liste aus."));
    }
    curl_global_cleanup();
    refreshServer();
}

void Lernapp::on_button4_5_clicked() { // Inhalt vom Server auflisten
    refreshServer();
}

void Lernapp::on_button4_6_clicked() // Upload
{
    CURL *curl;
    CURLcode res;
    FILE *file;

    QString ftpURL = "ftp://138.199.195.70:21/files/";
    QString uploadData = pathSystem + "/data_Lernapp/datenbank_Lernapp/" + selectedItemLocal;
    QString fileName = QFileInfo(uploadData).fileName();
    QString fullFtpURL = ftpURL + fileName;

    qDebug() << "Lokale Datei: " << uploadData;
    qDebug() << "FTP-Upload-Ziel: " << fullFtpURL;

    file = fopen(uploadData.toUtf8().constData(), "rb");
    if (!file) {
        QMessageBox::warning(nullptr, tr("Fehler"), tr("Datei konnte nicht geöffnet werden: %1").arg(uploadData));
        return;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);  // Hochladen aktivieren
        curl_easy_setopt(curl, CURLOPT_URL, fullFtpURL.toStdString().c_str());  // FTP-Ziel-URL mit Dateiname
        curl_easy_setopt(curl, CURLOPT_USERPWD, "bob:Kartoffel123?!"); // FTP-Login
        curl_easy_setopt(curl, CURLOPT_READDATA, file);  // Datei als Datenquelle setzen
        curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR); // Fehlende Ordner erstellen
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Debug-Ausgabe aktivieren

        // Upload starten
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            qWarning() << "Fehler beim Hochladen:" << curl_easy_strerror(res);
            QMessageBox::warning(nullptr, tr("Fehler"), tr("Upload fehlgeschlagen: %1").arg(curl_easy_strerror(res)));
        } else {
            QMessageBox::information(nullptr, tr("Erfolg"), tr("Datei erfolgreich hochgeladen."));
        }

        // Aufräumen
        fclose(file);
    } else {
        qWarning() << "Fehler: cURL konnte nicht initialisiert werden.";
        fclose(file);
    }
    curl_global_cleanup();
    refreshServer();
}

void Lernapp::on_button4_7_clicked() //Server-Datei löschen
{
    if(selectedItemServer != "") {
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Die Datei wird gelöscht!"), tr("Sind sie sicher, dass sie die Datei %1 löschen wollen?").arg(selectedItemServer), QMessageBox::Yes | QMessageBox::No);
        switch(ret) {
        case QMessageBox::No:
        {
            qDebug() << "No";
            break;
        }
        case QMessageBox::Yes:
        {
            qDebug() << "Yes";
            CURL* curl;
            CURLcode res;

            struct curl_slist* cmdlist = NULL;
            QString fileDeleteString = "DELE /files/" + selectedItemServer;
            const char* fileDeleteChar = fileDeleteString.toUtf8().constData();
            cmdlist = curl_slist_append(cmdlist, fileDeleteChar);

            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, ftpServerURL.toUtf8().constData());
                qDebug() << "URL: " << ftpServerURL;
                curl_easy_setopt(curl, CURLOPT_USERPWD, "bob:Kartoffel123?!");
                curl_easy_setopt(curl, CURLOPT_QUOTE, cmdlist);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                res = curl_easy_perform(curl);
                if(res != CURLE_OK) {
                    qDebug() << "Fehler: " << curl_easy_strerror(res);
                    QMessageBox::warning(nullptr, tr("Fehler beim löschen."), tr("%1").arg(curl_easy_strerror(res)));
                } else {
                    QMessageBox::warning(nullptr, tr("Datei erfolgreich gelöscht"), tr("%1 wurde gelöscht").arg(selectedItemServer));
                }
                curl_easy_cleanup(curl);
            }
            curl_slist_free_all(cmdlist);
            break;
        }
        default:
        {
            qDebug() << "Default case. What happened?";
        }
        }

    } else {
        QMessageBox::warning(nullptr, tr("Keine Datei ausgewählt."), tr("Bitte aktualisieren Sie die Liste und klicken Sie eine Datei aus."));
    }
    refreshServer();
}


//Seite 5

void Lernapp::on_button5_1_clicked()
{
    listDatabaseTableView(ui->tableView2_1);
    ui->tabWidget->setCurrentIndex(lastIndex);
}


void Lernapp::on_button5_2_clicked()
{
    QString buffer = activeDatabase;
    if(activeDatabase != "") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Datenbank auswählen", tr("Möchten Sie die Datenbank: %1\nals aktive Datenbank auswählen?").arg(activeDatabase), QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            ui->label2_1->setText(tr("Aktive Datenbank : %1").arg(activeDatabase));
            selectDatabase(activeDatabase);
            if(database.isValid()) {
                isDatabaseActive();
                if(lastIndex != 1 ) {
                    ui->tabWidget->setCurrentIndex(1);
                } else {
                    ui->tabWidget->setCurrentIndex(lastIndex);
                }
            } else {
                ui->tabWidget->setCurrentIndex(lastIndex);
                qDebug() << "Database not valid";
            }
        } else {
            activeDatabase = buffer;
        }
    } else {
        QMessageBox::warning(this, "Fehler", "Keine Datenbank ausgewählt.\nKlicken Sie eine Datenbank aus, bevor Sie auf auswählen klicken!");
    }
}

//Lernen
//Seite 6



//Seite 7



//Seite 8




// Debugging

void Lernapp::on_actionNext_triggered()
{
    ui->tabWidget->setCurrentIndex((ui->tabWidget->currentIndex()+1));
    QString site = "Seite: " + QString::number(ui->tabWidget->currentIndex());
    ui->menuSeite->setTitle(site);
}
void Lernapp::on_actionBack_triggered()
{
    ui->tabWidget->setCurrentIndex((ui->tabWidget->currentIndex()-1));
    QString site = "Seite: " + QString::number(ui->tabWidget->currentIndex());
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
void Lernapp::listDatabaseTableView(QTableView* tableView) //TableView mit Inhalt befüllen
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

    tableView->setModel(model); //Erstellen
    tableView->hideColumn(0);
    tableView->show();
}

QStringList Lernapp::parseFTPList(const QString &response) { //Output-Datei filtern
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

void Lernapp::setupDir() {

    QString pathDatenbank;

    qDebug() << pathSystem << "" << pathDatenbank;
    pathDatenbank = pathSystem + "/data_Lernapp";
    qDebug() << pathSystem << "" << pathDatenbank;

    QDir dir;
    if (!dir.exists(pathDatenbank)) {
        if (!dir.mkpath(pathDatenbank)) {
            qWarning() << "Fehler beim Erstellen des Ordners:" << pathDatenbank;
            QMessageBox::warning(nullptr, "Fehler beim Ertellen des Hauptverzeichnises", tr("Pfad: %1").arg(pathDatenbank));
            return;
        }
    }
}

void Lernapp::setupDatabaseDir()
{
    QString pathDatenbank;

    qDebug() << pathSystem << "" << pathDatenbank;
    pathDatenbank = pathSystem + "/data_Lernapp/datenbank_Lernapp";
    qDebug() << pathSystem << "" << pathDatenbank;

    QDir dir;
    if (!dir.exists(pathDatenbank)) {
        if (!dir.mkpath(pathDatenbank)) {
            qWarning() << "Fehler beim Erstellen des Ordners:" << pathDatenbank;
            QMessageBox::warning(nullptr, "Fehler beim Ertellen des Datenbankordners", tr("Pfad: %1").arg(pathDatenbank));
            return;
        }
    }
}

void Lernapp::setupDownloadDir()
{
    QString pathDatenbank;

    qDebug() << pathSystem << "" << pathDatenbank;
    pathDatenbank = pathSystem + "/data_Lernapp/download_Lernapp";
    qDebug() << pathSystem << "" << pathDatenbank;

    QDir dir;
    if (!dir.exists(pathDatenbank)) {
        if (!dir.mkpath(pathDatenbank)) {
            qWarning() << "Fehler beim Erstellen des Ordners:" << pathDatenbank;
            QMessageBox::warning(nullptr, "Fehler beim Ertellen des Downloadordners", tr("Pfad: %1").arg(pathDatenbank));
            return;
        }
    }
}

void Lernapp::listDataTreeRoot()
{
    QFileSystemModel *model = new QFileSystemModel;
    QString pathLocaleFiles = pathSystem + "/data_Lernapp";
    model->setRootPath(pathLocaleFiles);

    ui->treeView4_1->setModel(model);
    ui->treeView4_1->setRootIndex(model->index(pathLocaleFiles));
}

void Lernapp::listDatabaseTreeView(QTreeView* view)
{
    QFileSystemModel *model = new QFileSystemModel;
    QString pathLocaleFiles = pathSystem + "/data_Lernapp/datenbank_Lernapp";
    model->setRootPath(pathLocaleFiles);

    view->setModel(model);
    view->setRootIndex(model->index(pathLocaleFiles));
}

void Lernapp::treeView4ItemClicked(const QModelIndex &index)
{
    selectedItemLocal = index.data(Qt::DisplayRole).toString();
}

void Lernapp::treeView5ItemClicked(const QModelIndex &index)
{
    activeDatabase = index.data(Qt::DisplayRole).toString();
}

void Lernapp::selectDatabase(QString db)
{
    if(database.isOpen()) {
        database.close();
    }
    database.setDatabaseName(pathSystem + "/data_Lernapp/datenbank_Lernapp/" + db);

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

    listDatabaseTableView(ui->tableView2_1);
}

void Lernapp::isDatabaseActive()
{
    if(database.databaseName() != "") {
        ui->tabWidget->setTabEnabled(1,true);
        ui->tabWidget->setTabEnabled(2,true);
        ui->tabWidget->setTabEnabled(3,true);
        ui->tableView2_1->setDisabled(0);
        ui->button2_3->setDisabled(0);
        // ui->lineEdit2_3->setDisabled(0);
        // ui->button2_4->setDisabled(0);
        // ui->button_createNewDatabase->setDisabled(0);
        // ui->lineEdit_createNewDatabase->setDisabled(0);
        ui->radiobutton2_1->setDisabled(0);
        ui->radiobutton2_2->setDisabled(0);
        ui->textEdit2_1->setDisabled(0);
    } else {
        ui->tableView2_1->setDisabled(1);
        // ui->button2_3->setDisabled(1);
        // ui->lineEdit2_3->setDisabled(1);
        // ui->button2_4->setDisabled(1);
        // ui->button_createNewDatabase->setDisabled(1);
        // ui->lineEdit_createNewDatabase->setDisabled(1);
        ui->radiobutton2_1->setDisabled(1);
        ui->radiobutton2_2->setDisabled(1);
        ui->textEdit2_1->setDisabled(1);
    }
}

void Lernapp::disableTabs()
{
    ui->tabWidget->setTabVisible(0, false);
    ui->tabWidget->setTabVisible(1, false);
    ui->tabWidget->setTabVisible(2, false);
    ui->tabWidget->setTabVisible(3, false);
    ui->tabWidget->setTabVisible(4, false);
    ui->tabWidget->setTabVisible(5, false);
    ui->tabWidget->setTabVisible(6, false);
    ui->tabWidget->setTabVisible(7, false);
    ui->tabWidget->setTabVisible(8, false);
}

void Lernapp::lastSelectedTab(int index)
{
    if(lastIndexNow == lastIndex) {

    } else {
        lastIndex = lastIndexNow;
    }
    lastIndexNow = index;
}

void Lernapp::clickedServerTab()
{
    if(ui->tabWidget->currentIndex() == 3) {
        listDataTreeRoot();
        refreshServer();

        ui->treeWidget->setColumnCount(1);
        ui->treeWidget->setHeaderLabels(QStringList() << "FTP-Dateien");
    } else {

    }
}

void Lernapp::clickedUebersichtTab()
{
    listDatabaseTableView(ui->tableView3_1);
}

void Lernapp::refreshServer()
{
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabels(QStringList() << "FTP-Dateien");

    // FTP-Dateien abrufen
    QString ftpUrl = "ftp://138.199.195.70:21/files/";
    QString username = "bob";
    QString password = "Kartoffel123?!";
    QStringList files = getFTPFileList(ftpUrl, username, password);

    // Dateien zum TreeWidget hinzufügen
    for (const QString& file : files) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, file);
        ui->treeWidget->addTopLevelItem(item);
    }
}


void Lernapp::deleteDataEntryServer(QTreeWidgetItem *item, int index)
{
    selectedItemServer = item->text(index);
    qDebug() << selectedItemServer;
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

    fileList->append(parseFTPList(data));

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

        QString userPwd = username + ":" + password;
        curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.toStdString().c_str());

        // FTP Listing abrufen
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Lernapp::getDirFtp);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileList);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl Fehler: %s\n", curl_easy_strerror(res));
            QMessageBox::warning(nullptr, "Server Inhalt kann nicht aufgelistet werden", tr("%1").arg(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return fileList;
}
