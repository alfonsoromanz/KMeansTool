#include "mainwindow.h"
#include "clusterer.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icono.jppg"));
    MainWindow w;
    w.show();

    return a.exec();
}
