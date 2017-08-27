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

    Controller * controller = new Controller();

    controller->runClustering("g2.txt", 2, 1, 100, false);

    return a.exec();
}
