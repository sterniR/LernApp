#ifndef LERNAPP_H
#define LERNAPP_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDir>
#include <curl/curl.h>
#include <qboxlayout.h>
#include <QRegularExpression>
#include <QCoreApplication>
#include <QFileSystemModel>
#include <QTreeWidgetItem>

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
    QString frageText, frageTextKontrolle, erstelltVonText, istWahrText, selectedItemLocal, selectedItemServer;
    QString ftpServerURL = "ftp://138.199.195.70:21/files/";
    QString projektOrdner = QCoreApplication::applicationDirPath();
    QMessageBox msgBox;

    //Server cURL
    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t getDirFtp(void* contents, size_t size, size_t nmemb, void* userp);
    static QStringList getFTPFileList(const QString& ftpUrl, const QString& username, const QString& password);
    static QStringList parseFTPList(const QString &response);

private slots:

    //Database
    void error_database(QSqlDatabase);
    void error_query(QSqlError);

    //Funktionen
    void deleteDataEntryServer(QTreeWidgetItem* item, int index);
    void createDataEntry();
    void setupDatabaseDir();
    void listDataTree();
    void treeItemClicked(const QModelIndex &index);

    //GUI
    void on_button1_1_clicked();

    void on_button1_2_clicked();

    void on_button1_3_clicked();

    void on_button2_1_clicked();

    void on_button2_2_clicked();

    void on_button2_3_clicked();

    void on_button3_1_clicked();

    void on_button3_2_clicked();

    void on_button4_2_clicked();

    void on_button4_4_clicked();

    void on_button4_5_clicked();
    // Debugging
    void on_actionNext_triggered();

    void on_actionBack_triggered();


    void on_button4_6_clicked();

    void on_button4_7_clicked();

private:
    Ui::Lernapp *ui;
};
#endif // LERNAPP_H
