#include "mainwindow.h"
#include "controller.h"
#include <QApplication>
#include <mlpack/core.hpp>
#include <cmath>
#include <mlpack/methods/kmeans/kmeans.hpp>


using namespace mlpack;
using namespace mlpack::kmeans;
using namespace mlpack::metric;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Controller * controller = new Controller(&w);

    w.setController(controller);



    return a.exec();
}
