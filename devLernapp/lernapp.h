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

private:
    Ui::Lernapp *ui;
};
#endif // LERNAPP_H
