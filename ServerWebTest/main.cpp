#include "mainwindow.h"
#include "crow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load_text()
    });

    return a.exec();
}
