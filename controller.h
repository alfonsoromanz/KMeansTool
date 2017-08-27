#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <mlpack/core.hpp>
#include <cmath>
#include <mlpack/methods/kmeans/kmeans.hpp>


using namespace mlpack;
using namespace mlpack::kmeans;
using namespace mlpack::metric;

class Controller
{
public:
    Controller();
    void runClustering (const char* dataset, const size_t clusters, int metric, int maxIterations, bool testingMode);

};

#endif // CONTROLLER_H
