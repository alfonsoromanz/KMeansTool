#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <mlpack/core.hpp>
#include <cmath>
#include <QString>
#include <mlpack/methods/kmeans/kmeans.hpp>

class MainWindow;

using namespace mlpack;
using namespace mlpack::kmeans;
using namespace mlpack::metric;

class Controller
{
public:
    Controller(MainWindow * w);
    void runClustering (const char* dataset, const size_t clusters, int metric, int maxIterations, bool testingMode);

private:
   MainWindow * view;
};

#endif // CONTROLLER_H
