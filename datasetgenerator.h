#ifndef DATASETGENERATOR_H
#define DATASETGENERATOR_H
#include <mlpack/core.hpp>
#include <iostream>
#include <random>
#include <QString>


using namespace mlpack;

class DatasetGenerator
{
public:
    DatasetGenerator();
    bool createDataset (const QString &centersFile, long long int pointsPerCluster, long int error, const QString &fileOut);
};

#endif // DATASETGENERATOR_H