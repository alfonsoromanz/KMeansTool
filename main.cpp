#include "mainwindow.h"
#include "controller.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icono.jppg"));
    MainWindow w;
    w.show();

    Controller * controller = new Controller(&w);

    w.setController(controller);

    return a.exec();
}
