#include "datasetloader.h"

DatasetLoader::DatasetLoader(const QString &file, arma::mat * matrix, QProgressBar * progressBar)
{
    this->progressBar = progressBar;
    this->matrix = matrix;
    this->file = file;
}

void DatasetLoader::loadDataset()
{
    this->progressBar->setRange(0,0);
    this->progressBar->show();

    bool success = data::Load(file.toStdString().c_str(), *matrix, false);

    QString datasetDir;
    QString datasetName;


    if (success) {
        datasetDir = this->getDirectory(file);
        datasetName = this->getFileName(file);
    } else {
        datasetDir = "";
        datasetName = "";
    }

    this->progressBar->hide();
    emit finished(datasetDir, datasetName, success);
}


QString DatasetLoader::getDirectory(const QString &filePath) {
    QString ret = filePath;
    ret.remove(this->getFileName(filePath));
    return ret;
}


QString DatasetLoader::getFileName(const QString &filePath)
{
    QFileInfo i(filePath);
    QString ret = i.fileName();
    return ret;
}
