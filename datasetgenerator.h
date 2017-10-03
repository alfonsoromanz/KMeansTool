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
    bool createDataset (const QString &centersFile, int pointsPerCluster, int error);
};

#endif // DATASETGENERATOR_H
