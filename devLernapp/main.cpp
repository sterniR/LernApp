#include "lernapp.h"
#include <curl/curl.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lernapp w;
    w.show();
    return a.exec();
}
