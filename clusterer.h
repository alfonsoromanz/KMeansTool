#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <mlpack/core.hpp>
#include <QObject>
#include <cmath>
#include <mlpack/methods/kmeans/kmeans.hpp>
#include <mlpack/methods/kmeans/refined_start.hpp>
#include <QMap>
#include "fstream"
#include <ctime>
#include <QProgressBar>

class MainWindow;

using namespace mlpack;
using namespace mlpack::kmeans;
using namespace mlpack::metric;

class Clusterer : public QObject
{
    Q_OBJECT
public:
    Clusterer(arma::mat &dataset, const size_t clusters, int metric, int maxIterations, const QString &directory, const QString &file, bool testingMode);
    void createFiles();


public slots:
    void runClustering();

signals:
    void finished(QString message, bool success);

private:
   arma::mat dataset;
   size_t clusters;
   int metric;
   int maxIterations;
   QString directory;
   QString file;
   bool testingMode;

   QMap<int, arma::Col<double>> * calculateMeans (arma::mat data, arma::Row<size_t> originalAssignments);
   arma::Row<double> getMean (arma::mat data, size_t startCol, size_t endCol);
   QString pointToString(arma::mat &data, size_t pointColumn);
   QString colToString(const arma::Col<double> &data);
   QMap <int, int> clusterEquivalences(const QMap<int, arma::Col<double>> &originalCentroids, const arma::mat &newCentroids, int metric);
   double getDistance (const arma::Col<double> &pointA, const arma::Col<double> &pointB, int metric);
   void saveToFile (const std::string &path, QMap<int, arma::Col<double>> * dictionary);
};

#endif // CONTROLLER_H
