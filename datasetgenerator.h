#ifndef DATASETGENERATOR_H
#define DATASETGENERATOR_H
#include <mlpack/core.hpp>
#include <QApplication>
#include <iostream>
#include <random>
#include <QString>
#include <QObject>
#include <QProgressBar>

using namespace mlpack;

class DatasetGenerator: public QObject
{
    Q_OBJECT
public:
    DatasetGenerator(const QString &centersFile, long long int pointsPerCluster, long int error, bool gaussian, QString &fileOut, QProgressBar * progressBar);

public slots:
    void createDataset();

signals:
    void finished(QString fileOut, bool success);

private:
    QString centersFile;
    QString fileOut;
    long long int pointsPerCluster;
    long int error;
    bool gaussian;
    QProgressBar * progressBar;

};

#endif // DATASETGENERATOR_H
