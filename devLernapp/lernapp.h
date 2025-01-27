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

private slots:

    //Database
    void error_database(QSqlDatabase);
    void error_query(QSqlError);

    //Funktionen
    void createDataEntry();

    //GUI
    void on_button1_1_clicked();

    void on_button1_2_clicked();

    void on_button1_3_clicked();

    void on_button2_1_clicked();

    void on_button2_2_clicked();

    void on_button2_3_clicked();

    void on_button3_1_clicked();

    void on_button4_2_clicked();

    void on_button3_2_clicked();

    // Debugging
    void on_actionNext_triggered();

    void on_actionBack_triggered();
private:
    Ui::Lernapp *ui;
};
#endif // LERNAPP_H
