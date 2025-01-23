#include "lernapp.h"
#include "./ui_lernapp.h"

Lernapp::Lernapp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lernapp)
{
    ui->setupUi(this);
}

Lernapp::~Lernapp()
{
    delete ui;
}

//Seite 0

void Lernapp::on_button1_1_clicked()
{

}

void Lernapp::on_button1_2_clicked()
{

}

void Lernapp::on_button1_3_clicked()
{

}
//Seite 1

//Seite 2

//Seite 3

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





