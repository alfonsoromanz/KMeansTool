#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <mlpack/core.hpp>
#include <cmath>
#include <QString>
#include <mlpack/methods/kmeans/kmeans.hpp>
#include <QVector>
#include <QMap>
#include "sstream"
#include "fstream"

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
   QMap<int, arma::Col<double>> calculateMeans (arma::mat data, arma::Row<size_t> originalAssignments);
   arma::Row<double> getMean (arma::mat data, size_t startCol, size_t endCol);
   QString pointToString(arma::mat &data, size_t pointColumn);
   QString colToString(const arma::Col<double> &data);
   QMap <int, int> clusterEquivalences(const QMap<int, arma::Col<double>> &originalCentroids, const arma::mat &newCentroids, int metric);
   double getDistance (const arma::Col<double> &pointA, const arma::Col<double> &pointB, int metric);
};

#endif // CONTROLLER_H
