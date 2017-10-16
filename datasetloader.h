#ifndef DATASETLOADER_H
#define DATASETLOADER_H

#include <QObject>
#include <QString>
#include <mlpack/core.hpp>
#include <QProgressBar>
#include <QFileInfo>

using namespace mlpack;


class DatasetLoader: public QObject
{
    Q_OBJECT

public:
    DatasetLoader(const QString &file, arma::mat * matrix, QProgressBar * progressBar);

public slots:
    void loadDataset();

signals:
    void finished(QString datasetDir, QString datasetName, bool success);

private:
    arma::mat * matrix;
    QString file;
    QProgressBar * progressBar;

    QString getDirectory (const QString & filePath);
    QString getFileName (const QString & filePath);
};

#endif // DATASETLOADER_H
