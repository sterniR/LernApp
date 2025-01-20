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
