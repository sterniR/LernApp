#ifndef LERNAPP_H
#define LERNAPP_H

#include <QMainWindow>

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

private slots:
    void on_actionNext_triggered();

    void on_actionBack_triggered();

    void on_button1_1_clicked();

    void on_button1_2_clicked();

    void on_button1_3_clicked();

private:
    Ui::Lernapp *ui;
};
#endif // LERNAPP_H
