#include "mainwindow.h"
#include "controller.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Controller * controller = new Controller(&w);

    w.setController(controller);

    return a.exec();
}
