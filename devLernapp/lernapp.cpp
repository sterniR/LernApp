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
    ui->button_home->hide();


    connect(ui->treeView4_1, &QTreeView::clicked, this, &Lernapp::treeView4ItemClicked);
    connect(ui->treeView5_1, &QTreeView::clicked, this, &Lernapp::treeView5ItemClicked);
    connect(ui->treeView5_1, &QTreeView::doubleClicked, this, &Lernapp::treeView5ItemdoubleClicked);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &Lernapp::itemClickedTreeView);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &Lernapp::lastSelectedTab);
    connect(ui->tabWidget_2, &QTabWidget::currentChanged, this, &Lernapp::lastTabWidget2Clicked);
    connect(ui->lineEdit2_1, &QLineEdit::editingFinished, this, &Lernapp::frageInputFinished);
    connect(ui->checkBox_antwort1, &QCheckBox::checkStateChanged, this, &Lernapp::checkboxStateChanged);
    connect(ui->checkBox_antwort2, &QCheckBox::checkStateChanged, this, &Lernapp::checkboxStateChanged);
    connect(ui->checkBox_antwort3, &QCheckBox::checkStateChanged, this, &Lernapp::checkboxStateChanged);

    setupDir();
    setupDatabaseDir();
    database = QSqlDatabase::addDatabase("QSQLITE");
}

Lernapp::~Lernapp()
{
    delete ui;
}

//Startseite

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

void Lernapp::on_button2_3_clicked() // Speichern
{
    frageText = ui->textEdit2_1->toPlainText();
    erstelltVonText = ui->lineEdit2_1->text();

    /*istWahrText
     *
     * istWahrMitText
     * istFalschMitText
     * istFalsch2MitText
     *
     * antwortMitEingabeString
     * */


    if(frageText == "") {
        // QMessageBox::warning(this,"Keine Frage gefunden!","Bitte fügen Sie eine Fragen hinzu \nbevor Sie auf Speicher klicken.");
        ui->label_saveNotification->setText("Keine Frage gefunden!\nBitte fügen Sie eine Fragen hinzu \nbevor Sie auf 'Speichern' klicken.");
    } else if(frageTextKontrolle == frageText) {
        // QMessageBox::warning(this,"Gleiche Frage","Diese Frage existiert schon.\nBitte ändern Sie die Frage.");
        ui->label_saveNotification->setText("Gleiche Frage.\nDiese Frage existiert schon.\nBitte ändern Sie die Frage.");
    } else {
        switch (statusOption.toInt()) {
        case 1:
            qDebug() << statusOption;
            istWahrMitText = "";
            istFalschMitText = "";
            istFalsch2MitText = "";
            antwortMitEingabeString = "";
            if(ui->radioButton2_1->isChecked() == false && ui->radioButton2_2->isChecked() == false) {
                ui->label_saveNotification->setText("Bitte wählen Sie ein Wert (Richtig oder Flasch) aus.");
            } else if(ui->radioButton2_1->isChecked()) {
                istWahrText = "richtig";
                insertIntoTableDatabase();
                ui->label_saveNotification->clear();
            } else if(ui->radioButton2_2->isChecked()) {
                istWahrText = "falsch";
                insertIntoTableDatabase();
                ui->label_saveNotification->clear();
            }
            break;
        case 2:
            qDebug() << statusOption;
            istWahrText = "";
            antwortMitEingabeString = "";

            if(checkboxCounter <= 1) {
                ui->label_saveNotification->setText("Es müssen mindestends 2 Antworten geben.");
            } else if(checkboxCounter >= 2) {
                ui->label_saveNotification->clear();

                if(ui->radioButton2_3->isChecked()) {
                    if(ui->lineEdit_2->text() == "") {
                        ui->label_saveNotification->setText("Bitte fügen Sie für 1. Antwort ein Text hinzu.");
                    } else if(ui->radioButton2_5->isChecked() || ui->radioButton2_7->isChecked() || (ui->radioButton2_5->isChecked() && ui->radioButton2_7->isChecked())) {
                        ui->label_saveNotification->setText("Es kann nur eine richtige Antwort geben.");
                    } else {
                        istWahrMitText = ui->lineEdit_2->text();
                        istFalschMitText = ui->lineEdit->text();
                        istFalsch2MitText = ui->lineEdit_3->text();
                        insertIntoTableDatabase();
                        ui->label_saveNotification->clear();
                    }
                } else if (ui->radioButton2_5->isChecked()){
                    if(ui->lineEdit->text() == "") {
                        ui->label_saveNotification->setText("Bitte fügen Sie für 1. Antwort ein Text hinzu.");
                    } else if(ui->radioButton2_3->isChecked() || ui->radioButton2_7->isChecked() || (ui->radioButton2_3->isChecked() && ui->radioButton2_7->isChecked())) {
                        ui->label_saveNotification->setText("Es kann nur eine richtige Antwort geben.");
                    } else {
                        istWahrMitText = ui->lineEdit->text();
                        istFalschMitText = ui->lineEdit_2->text();
                        istFalsch2MitText = ui->lineEdit_3->text();
                        insertIntoTableDatabase();
                        ui->label_saveNotification->clear();
                    }
                } else if(ui->radioButton2_7->isChecked()) {
                    if(ui->lineEdit_3->text() == "") {
                        ui->label_saveNotification->setText("Bitte fügen Sie für 1. Antwort ein Text hinzu.");
                    } else if(ui->radioButton2_5->isChecked() || ui->radioButton2_2->isChecked() || (ui->radioButton2_5->isChecked() && ui->radioButton2_2->isChecked())) {
                        ui->label_saveNotification->setText("Es kann nur eine richtige Antwort geben.");
                    } else {
                        istWahrMitText = ui->lineEdit_3->text();
                        istFalschMitText = ui->lineEdit_2->text();
                        istFalsch2MitText = ui->lineEdit->text();
                        insertIntoTableDatabase();
                        ui->label_saveNotification->clear();
                    }
                } else {
                    ui->label_saveNotification->setText("Bitte wählen Sie eine Antwort als richtig aus.");
                }
            }




            break;
        case 3:
            qDebug() << statusOption;
            istWahrMitText = "";
            istFalschMitText = "";
            istFalsch2MitText = "";
            istWahrText = "";

            if(ui->lineEdit_4->text() == "") {
                ui->label_saveNotification->setText("Keine Wort/Begriff gefunden!\nBitte fügen Sie eine Wort/Begriff hinzu \nbevor Sie auf 'Speichern' klicken.");
            } else if(ui->lineEdit_4->text() != "") {
                antwortMitEingabeString = ui->lineEdit_4->text();
                insertIntoTableDatabase();
                ui->label_saveNotification->clear();
            }
            break;
        default:

            break;
        }
    }

}
//Seite - Editor

// void Lernapp::insertIntoTableDatabase() // SQL-Datenbank - Insert
// {
//     QSqlQuery query(database);
//     query.prepare("INSERT OR IGNORE INTO Fragen ("
//                   "frage_text, ist_wahr, ist_wahr_mit_text, ist_falsch_mit_text, ist_falsch2_mit_text, antwort_mit_eingabe, status, erstellt_von)"
//                   "VALUES ('" +
//                   frageText + "', '" + istWahrText + "', '" +
//                   istWahrMitText + "', '" + istFalschMitText + "', '" + istFalsch2MitText + "', '" +
//                   antwortMitEingabeString + "', '" +
//                   statusOption + "', '" + erstelltVonText +
//                   "');");
//     if(!query.exec())
//         error_query(query.lastError());
//     listDatabaseTableView(ui->tableView2_1);
// }

void Lernapp::on_button_home_clicked() // Button - Startseite
{
    if(ui->tabWidget->currentIndex() == 0) {
        ui->button_home->hide();
    } else {
        ui->button_home->show();
        disableTabs();
        ui->tabWidget->setCurrentIndex(0);
    }
}



void Lernapp::checkboxStateChanged(Qt::CheckState state) // Checkboxes Clicked
{
    if(ui->checkBox_antwort1->isChecked()) { //Checkbox 1
        ui->radioButton2_3->setDisabled(false);
        ui->radioButton2_4->setDisabled(false);
        ui->radioButton2_3->setAutoExclusive(true);
        ui->radioButton2_4->setAutoExclusive(true);
        ui->lineEdit_2->setDisabled(false);
        ui->radioButton2_4->setChecked(1);
    } else {
        ui->radioButton2_3->setAutoExclusive(false);
        ui->radioButton2_4->setAutoExclusive(false);
        ui->radioButton2_3->setChecked(0);
        ui->radioButton2_4->setChecked(0);
        ui->radioButton2_3->setDisabled(true);
        ui->radioButton2_4->setDisabled(true);
        ui->lineEdit_2->setText("");
        ui->lineEdit_2->setDisabled(true);
    }

    if(ui->checkBox_antwort2->isChecked()) { //Checkbox 2
        ui->radioButton2_5->setDisabled(false);
        ui->radioButton2_6->setDisabled(false);
        ui->radioButton2_5->setAutoExclusive(true);
        ui->radioButton2_6->setAutoExclusive(true);
        ui->lineEdit->setDisabled(false);
        ui->radioButton2_6->setChecked(1);
    } else {
        ui->radioButton2_5->setAutoExclusive(false);
        ui->radioButton2_6->setAutoExclusive(false);
        ui->radioButton2_5->setChecked(0);
        ui->radioButton2_6->setChecked(0);
        ui->radioButton2_5->setDisabled(true);
        ui->radioButton2_6->setDisabled(true);
        ui->lineEdit->setText("");
        ui->lineEdit->setDisabled(true);
    }

    if(ui->checkBox_antwort3->isChecked()) { //Checkbox 3
        ui->radioButton2_7->setDisabled(false);
        ui->radioButton2_8->setDisabled(false);
        ui->radioButton2_7->setAutoExclusive(true);
        ui->radioButton2_8->setAutoExclusive(true);
        ui->lineEdit_3->setDisabled(false);
        ui->radioButton2_8->setChecked(1);
    } else {
        ui->radioButton2_7->setAutoExclusive(false);
        ui->radioButton2_8->setAutoExclusive(false);
        ui->radioButton2_7->setChecked(0);
        ui->radioButton2_8->setChecked(0);
        ui->radioButton2_7->setDisabled(true);
        ui->radioButton2_8->setDisabled(true);
        ui->lineEdit_3->setText("");
        ui->lineEdit_3->setDisabled(true);
    }

    if(state == 2) {
        checkboxCounter++;
        qDebug() << checkboxCounter;
    } else if(state == 0){
        checkboxCounter--;
        qDebug() << checkboxCounter;
    }
}

void Lernapp::on_button2_4_clicked() //Datenbank umbenennen
{
    // database.close() und activateDatabase.close() oder so. Problem bei windows ## FIX ME!
    if(database.isOpen()) {
        database.close();
    }
    QFile file;
    QMessageBox::StandardButton reply;
    if(selectedItemLocal != "") {
        reply = QMessageBox::question(this,"Datenbank wird umbennant!", tr("Die Datei %1 wird zu %2.\nFortfahren?").arg(selectedItemLocal, ui->lineEdit2_3->text()), QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == reply) {
            if(QDir::setCurrent(pathSystem + "/data_Lernapp/datenbank_Lernapp")) {
                file.setFileName(selectedItemLocal);
                if(!file.rename(ui->lineEdit2_3->text())) {
                    QMessageBox::warning(this, "Fehler", tr("Datenbank konnte nicht umbenannt werden.\n%1").arg(file.errorString()));
                    qDebug() << file.fileName();
                } else {
                    ui->label2_1->setText(tr("Aktive Datenbank: %1").arg(ui->lineEdit2_3->text()));
                    listDatabaseTableView(ui->tableView2_1);
                    ui->button5_2->setDisabled(1);
                    ui->lineEdit2_3->setDisabled(1);
                    ui->button2_4->setDisabled(1);
                    ui->tabWidget->setTabEnabled(1, false);
                    ui->tabWidget->setTabEnabled(2, false);
                    listDatabaseTreeView(ui->treeView5_1);
                    selectedItemLocal.clear();
                }
            } else {
                QMessageBox::warning(this, tr("Konnte nicht gelöscht werden!"), tr("%1").arg(file.errorString()));
            }
        }
    } else {
        QMessageBox::warning(this, "Fehler", "Bitte wählen Sie eine Datenbank vorher aus.");
    }
    ui->lineEdit2_3->clear();
}

void Lernapp::on_button_createNewDatabase_clicked() //Neue Datenbank erstellen
{
    QString createdDatabaseText = ui->lineEdit_createNewDatabase->text();
    selectDatabase(createdDatabaseText);
    createdDatabaseText.clear();
    ui->lineEdit_createNewDatabase->clear();
}

void Lernapp::on_button_deleteDatabase_clicked() // Lokale Datenbank löschen
{
    // database.close() und activateDatabase.close() oder so. Problem bei windows ## FIX ME!
    database.close();
    QFile db;
    QMessageBox::StandardButton reply;
    if(selectedItemLocal != "") {
        reply = QMessageBox::question(this,"Datenbank wird gelöschen!", tr("Die Datei %1 wird entgültig vom System gelöscht.\nFortfahren?").arg(selectedItemLocal), QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == reply) {
            if(QDir::setCurrent(pathSystem + "/data_Lernapp/datenbank_Lernapp")) {
                db.setFileName(selectedItemLocal);
                db.remove();
                ui->button5_2->setDisabled(1);
                ui->lineEdit2_3->setDisabled(1);
                ui->button2_4->setDisabled(1);
                ui->tabWidget->setTabEnabled(1,0);
                ui->tabWidget->setTabEnabled(2,0);
                listDatabaseTreeView(ui->treeView5_1);
                selectedItemLocal.clear();

            } else {
                QMessageBox::warning(this, tr("Konnte nicht gelöscht werden!"), tr("%1").arg(db.errorString()));
            }
        } else {
            listDatabaseTreeView(ui->treeView5_1);
            ui->button5_2->setDisabled(1);
            ui->lineEdit2_3->setDisabled(1);
            ui->button2_4->setDisabled(1);
            selectedItemLocal.clear();
        }
    } else {
        QMessageBox::information(this, tr("Keine Datenbank ausgewählt."), "Bitte wählen Sie eine Datenbank aus.");
    }
}

void Lernapp::on_button4_4_clicked() //Download
{
    CURL *curl;
    CURLcode res;

    QString ftpUrl = "ftp://138.199.195.70:21/files/";

    if(ui->treeWidget->currentItem() != NULL) {
        QTreeWidgetItem *selectedFile = ui->treeWidget->currentItem();
        QString cleanText = selectedFile->text(ui->treeWidget->currentColumn());
        QString itemText = cleanText.replace("\r","");

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

void Lernapp::on_button5_2_clicked() // Datenbank öffnen
{
    QString buffer = selectedItemLocal;
    if(selectedItemLocal != "") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Datenbank auswählen", tr("Möchten Sie die Datenbank: %1\nals aktive Datenbank auswählen?").arg(selectedItemLocal), QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            ui->label2_1->setText(tr("Aktive Datenbank : %1").arg(selectedItemLocal));
            ui->textEdit2_1->clear();
            selectDatabase(selectedItemLocal);
            activeDatabase = selectedItemLocal;
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
            selectedItemLocal = buffer;
            ui->button5_2->setDisabled(1);
            ui->lineEdit2_3->setDisabled(1);
            ui->button2_4->setDisabled(1);
            selectedItemLocal.clear();
            listDatabaseTreeView(ui->treeView5_1);
        }
    } else {
        QMessageBox::warning(this, "Fehler", "Keine Datenbank ausgewählt.\nKlicken Sie eine Datenbank aus, bevor Sie auf auswählen klicken!");
    }
}

void Lernapp::treeView5ItemdoubleClicked(const QModelIndex &index) //Datenbank öffnen (Double Click)
{
    selectedItemLocal = index.data(Qt::DisplayRole).toString();
    QString buffer = selectedItemLocal;
    if(selectedItemLocal != "") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Datenbank auswählen", tr("Möchten Sie die Datenbank: %1\nals aktive Datenbank auswählen?").arg(selectedItemLocal), QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            ui->label2_1->setText(tr("Aktive Datenbank : %1").arg(selectedItemLocal));
            ui->textEdit2_1->clear();
            selectDatabase(selectedItemLocal);
            activeDatabase = selectedItemLocal;
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
            selectedItemLocal = buffer;
            ui->button5_2->setDisabled(1);
            ui->lineEdit2_3->setDisabled(1);
            ui->button2_4->setDisabled(1);
            selectedItemLocal.clear();
            listDatabaseTreeView(ui->treeView5_1);
        }
    } else {
        QMessageBox::warning(this, "Fehler", "Keine Datenbank ausgewählt.\nKlicken Sie eine Datenbank aus, bevor Sie auf auswählen klicken!");
    }
}

void Lernapp::lastTabWidget2Clicked(int index) // TabWidget2 clicked
{
    switch (index) {
    case 0:
        statusOption = "1";
        break;
    case 1:
        statusOption = "2";
        break;
    case 2:
        statusOption = "3";
        break;
    default:
        break;
    }
}

//Seite - Lernen


// Debugging

// void Lernapp::on_actionNext_triggered()
// {
//     ui->tabWidget->setCurrentIndex((ui->tabWidget->currentIndex()+1));
//     QString site = "Seite: " + QString::number(ui->tabWidget->currentIndex());
//     ui->menuSeite->setTitle(site);
// }
// void Lernapp::on_actionBack_triggered()
// {
//     ui->tabWidget->setCurrentIndex((ui->tabWidget->currentIndex()-1));
//     QString site = "Seite: " + QString::number(ui->tabWidget->currentIndex());
//     ui->menuSeite->setTitle(site);
// }

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

    QSqlTableModel *model = new QSqlTableModel(this, database);
    model->setTable("Fragen");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Frage"));
    model->setHeaderData(2, Qt::Horizontal, tr("Wahr/Falsch"));
    model->setHeaderData(3, Qt::Horizontal, tr("Antworten(1-3) - Wahr"));
    model->setHeaderData(4, Qt::Horizontal, tr("Antworten(1-3) - Falsch"));
    model->setHeaderData(5, Qt::Horizontal, tr("Antworten(1-3) - Falsch 2"));
    model->setHeaderData(6, Qt::Horizontal, tr("Ein Wort"));
    model->setHeaderData(7, Qt::Horizontal, tr("Status"));
    model->setHeaderData(8, Qt::Horizontal, tr("Erstellt von"));
    model->setHeaderData(9, Qt::Horizontal, tr("Erstellt am"));

    tableView->setModel(model);
    tableView->hideColumn(0);
    tableView->hideColumn(10);
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

void Lernapp::setupDir() { //Erstellt lokales Verzeichnis

    QString pathDatenbank;
    pathDatenbank = pathSystem + "/data_Lernapp";
    QDir dir;
    if (!dir.exists(pathDatenbank)) {
        if (!dir.mkpath(pathDatenbank)) {
            qWarning() << "Fehler beim Erstellen des Ordners:" << pathDatenbank;
            QMessageBox::warning(nullptr, "Fehler beim Ertellen des Hauptverzeichnises", tr("Pfad: %1").arg(pathDatenbank));
            return;
        }
    }
}

void Lernapp::setupDatabaseDir() //Erstellte lokale Ordner für die Datenbank
{
    QString pathDatenbank;
    pathDatenbank = pathSystem + "/data_Lernapp/datenbank_Lernapp";

    QDir dir;
    if (!dir.exists(pathDatenbank)) {
        if (!dir.mkpath(pathDatenbank)) {
            qWarning() << "Fehler beim Erstellen des Ordners:" << pathDatenbank;
            QMessageBox::warning(nullptr, "Fehler beim Ertellen des Datenbankordners", tr("Pfad: %1").arg(pathDatenbank));
            return;
        }
    }
}

void Lernapp::setupDownloadDir() // Not used anymore
{
    QString pathDatenbank;
    pathDatenbank = pathSystem + "/data_Lernapp/download_Lernapp";

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

void Lernapp::listDatabaseTreeView(QTreeView* view) //QTreeView vom lokalen Verzeichnis mit Datein vom der Datenbank anzeigen
{
    QFileSystemModel *model = new QFileSystemModel;
    QString pathLocaleFiles = pathSystem + "/data_Lernapp/datenbank_Lernapp";
    model->setRootPath(pathLocaleFiles);

    view->setModel(model);
    view->setRootIndex(model->index(pathLocaleFiles));
    view->hideColumn(1);
    view->hideColumn(2);
    view->hideColumn(3);
}

void Lernapp::treeView4ItemClicked(const QModelIndex &index)
{
    selectedItemLocal = index.data(Qt::DisplayRole).toString();
}

void Lernapp::treeView5ItemClicked(const QModelIndex &index)
{
    selectedItemLocal = index.data(Qt::DisplayRole).toString();

    if(selectedItemLocal != "") {
        ui->button5_2->setDisabled(0);
        ui->lineEdit2_3->setDisabled(0);
        ui->button2_4->setDisabled(0);
    } else {
        ui->button5_2->setDisabled(1);
        ui->lineEdit2_3->setDisabled(1);
        ui->button2_4->setDisabled(1);
        listDatabaseTreeView(ui->treeView5_1);
    }

}

void Lernapp::selectDatabase(QString db) // SQL-Datenbank - Create new Table
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
                    "ist_wahr TEXT NOT NULL,"
                    "ist_wahr_mit_text TEXT NOT NULL,"
                    "ist_falsch_mit_text TEXT NOT NULL,"
                    "ist_falsch2_mit_text TEXT NOT NULL,"
                    "antwort_mit_eingabe TEXT NOT NULL,"
                    "status INTEGER CHECK (status IN (1, 2, 3)),"
                    "erstellt_von TEXT NOT NULL,"
                    "erstellt_am TIMESTAMP DEFAULT CURRENT_TIMESTAMP);");
    if(!query.exec())
        error_query(query.lastError());

    listDatabaseTableView(ui->tableView2_1);
}

void Lernapp::insertIntoTableDatabase() // SQL-Datenbank - Insert
{
    QSqlQuery query(database);
    query.prepare("INSERT OR IGNORE INTO Fragen ("
                  "frage_text, ist_wahr, ist_wahr_mit_text, ist_falsch_mit_text, ist_falsch2_mit_text, antwort_mit_eingabe, status, erstellt_von)"
                  "VALUES ('" +
                  frageText + "', '" + istWahrText + "', '" +
                  istWahrMitText + "', '" + istFalschMitText + "', '" + istFalsch2MitText + "', '" +
                  antwortMitEingabeString + "', '" +
                  statusOption + "', '" + erstelltVonText +
                  "');");
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
        ui->radioButton2_1->setDisabled(0);
        ui->radioButton2_2->setDisabled(0);
        ui->textEdit2_1->setDisabled(0);
    } else {
        ui->tableView2_1->setDisabled(1);
        ui->radioButton2_1->setDisabled(1);
        ui->radioButton2_2->setDisabled(1);
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

    if(ui->tabWidget->currentIndex() == 3) { // Server-Tab
        listDatabaseTreeView(ui->treeView4_1);
        refreshServer();
        ui->treeWidget->setColumnCount(1);
        ui->button_home->show();
    } else if(ui->tabWidget->currentIndex() == 2) { // Uebersicht-Tab
        listDatabaseTableView(ui->tableView3_1);
        ui->button_home->show();
    } else if (ui->tabWidget->currentIndex() == 4) { // Lokale Datenbank auswählen-Tab
        if(selectedItemLocal == "") {
            ui->button5_2->setDisabled(1);
            ui->lineEdit2_3->setDisabled(1);
            ui->button2_4->setDisabled(1);
        }
        ui->button_home->show();
    } else if(ui->tabWidget->currentIndex() == 0) { // Startseite-Tab
        ui->button_home->hide();
    } else if(ui->tabWidget->currentIndex() == 1) { // Bearbeiten-Tab
        ui->button_home->show();
        ui->tabWidget_2->setCurrentIndex(0);
        ui->label2_1->setText(tr("Aktive Datenbank: %1").arg(activeDatabase));
        selectDatabase(activeDatabase);
        listDatabaseTableView(ui->tableView2_1);
    } else {
        ui->button_home->show();
        ui->button5_2->setDisabled(0);
        ui->lineEdit2_3->setDisabled(0);
        ui->button2_4->setDisabled(0);
    }

}

void Lernapp::refreshServer() // Server-Dateien Aktualisieren
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

void Lernapp::itemClickedTreeView(QTreeWidgetItem *item, int index)
{
    selectedItemLocal = item->text(index);
    selectedItemServer = item->text(index);
    qDebug() << selectedItemServer;
    qDebug() << selectedItemLocal;
}

void Lernapp::frageInputFinished() // Frage Line Edit Finished
{
    // frageTextKontrolle = ui->textEdit2_1->toPlainText();
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
