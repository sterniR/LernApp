#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openButton_clicked()
{

}


void MainWindow::on_uploadButton_clicked()
{

}


void MainWindow::on_setFolderButton_clicked()
{

}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

}

