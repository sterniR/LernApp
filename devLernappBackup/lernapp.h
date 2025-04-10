#ifndef LERNAPP_H
#define LERNAPP_H

#include <QMainWindow>
#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>

#include <QDir>
#include <curl/curl.h>
#include <qboxlayout.h>

#include <QRegularExpression>
#include <QFileSystemModel>
#include <QTreeWidgetItem>
#include <QTableView>
#include <QStandardPaths>
#include <QFile>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class Lernapp;
}
QT_END_NAMESPACE

class Lernapp : public QMainWindow
{
    Q_OBJECT

public:
    Lernapp(QWidget *parent = nullptr);
    ~Lernapp();

    QSqlDatabase database;
    QString frageText, frageTextKontrolle, erstelltVonText, istWahrText, selectedItemLocal,
        selectedItemServer, activeDatabase, statusOption = "1", antwortMitEingabeString,
        istWahrMitText, istFalschMitText, istFalsch2MitText;
    QString ftpServerURL = "ftp://138.199.195.70:21/files/";
    QString const pathSystem = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QMessageBox msgBox;
    int lastIndex = 0, lastIndexNow = 0, checkboxCounter = 1;

    //Server cURL
    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t getDirFtp(void* contents, size_t size, size_t nmemb, void* userp);
    static QStringList getFTPFileList(const QString& ftpUrl, const QString& username, const QString& password);
    static QStringList parseFTPList(const QString &response);

private slots:

    //Database
    void error_database(QSqlDatabase);
    void error_query(QSqlError);
    void insertIntoTableDatabase();

    //Funktionen
    void itemClickedTreeView(QTreeWidgetItem* item, int index);
    void frageInputFinished();

    void setupDir();
    void setupDatabaseDir();
    void setupDownloadDir();

    void listDataTreeRoot();
    void listDatabaseTableView(QTableView* tableView);
    void listDatabaseTreeView(QTreeView* view);

    void treeView4ItemClicked(const QModelIndex &index);
    void treeView5ItemClicked(const QModelIndex &index);
    void treeView5ItemdoubleClicked(const QModelIndex &index);

    void selectDatabase(QString db);
    void isDatabaseActive();

    void disableTabs();

    void refreshServer();

    //GUI

    void lastSelectedTab(int index); //TabWidget 1
    void lastTabWidget2Clicked(int index); ////TabWidget 2

    void on_button_home_clicked();

    // Tab 0 Startseite
    void on_button_editorStart_clicked();

    // Tab 1 Bearbeiten
    void on_button2_3_clicked();
    void on_button2_4_clicked();
    void on_button_createNewDatabase_clicked();
    void checkboxStateChanged(Qt::CheckState state);

    void on_button4_4_clicked();
    void on_button4_5_clicked();
    void on_button4_6_clicked();
    void on_button4_7_clicked();

    void on_button5_2_clicked();



    // Debugging
    void on_actionNext_triggered();
    void on_actionBack_triggered();


    void on_button_deleteDatabase_clicked();



private:
    Ui::Lernapp *ui;
};
#endif // LERNAPP_H
