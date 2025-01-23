#include "lernapp.h"
#include "./ui_lernapp.h"

Lernapp::Lernapp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lernapp)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

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

//Seite 0

void Lernapp::on_button1_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void Lernapp::on_button1_2_clicked()
{
    createDataEntry();
    ui->stackedWidget->setCurrentIndex(2);
}
void Lernapp::on_button1_3_clicked()
{

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
    QString frageText = ui->textEdit2_1->toPlainText();
    QString erstelltVonText = ui->lineEdit2_1->displayText();
    QString istWahrText;

    if(ui->radiobutton2_1->isChecked())
    {
        istWahrText = "1";
    }
    else if(ui->radiobutton2_2->isChecked())
    {
        istWahrText = "0";
    }
    else
    {
        QMessageBox::warning(this,"Achtung", "Bitte 'Wahr' oder 'Falsch' auswaehlen.");
    }

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
    QMessageBox::warning(nullptr, tr("Fehler Query"),tr("Fehler: %1").arg(error.lastError().text()));
}
void Lernapp::error_query(QSqlError error)
{
    QMessageBox::warning(nullptr, tr("Fehler Database"),tr("Fehler: %1").arg(error.text()));

}

//Funktionen
void Lernapp::createDataEntry()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT frage_text, ist_wahr, erstellt_von FROM Fragen");
    model->setHeaderData(0, Qt::Horizontal, tr("frage_text"));
    model->setHeaderData(1, Qt::Horizontal, tr("ist_wahr"));
    model->setHeaderData(2, Qt::Horizontal, tr("erstellt_von"));
    //model->setHeaderData(3, Qt::Horizontal, tr("erstellt_am")); //Datum
    ui->tableView3_1->setModel(model);
    ui->tableView3_1->show();
}



