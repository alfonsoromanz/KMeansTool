#include "datasetgenerator.h"

DatasetGenerator::DatasetGenerator()
{

}

bool DatasetGenerator::createDataset(const QString &centersFile, long long int pointsPerCluster, long int error, bool gaussian, QString &fileOut)
{
    bool success = 1;
    arma::imat centers;
    success = data::Load(centersFile.toStdString().c_str(), centers, false);

    if (!success) {
        return 0;
    }

    arma::imat dataset;
    int n_clusters = centers.n_cols;
    dataset.zeros(centers.n_rows+1, n_clusters * pointsPerCluster);
    double stddev = sqrt(error);

    int rows = centers.n_rows+1;
    int cols = n_clusters * pointsPerCluster;
    std::random_device rd;
    std::default_random_engine generator(rd());
    for (long int c=0; c<cols; c++) {
        int cluster = floor(c/pointsPerCluster);
        //set the label
        dataset(0,c)=cluster;
        //generate point
        for (long int r=1; r<rows; r++) {
            if (gaussian) {
                std::normal_distribution<> distribution(centers(r-1,cluster), stddev);
                dataset(r, c) = distribution(generator);
            } else {
                std::uniform_int_distribution<> distribution(centers(r-1,cluster)-error, centers(r-1,cluster)+error);
                dataset(r, c) = distribution(generator);
            }
        }
    }
    //test
    QString file (fileOut);

    //add prefix
    QString prefix = gaussian ? "_normal.txt" : "_uniforme.txt";
    fileOut = fileOut + prefix;

    data::Save(fileOut.toStdString().c_str(), dataset, true);

    arma::imat newmat;
    newmat.zeros(dataset.n_rows-1, dataset.n_cols);
    for (long int i=0; i<newmat.n_rows; i++) {
        for (long int j=0; j<newmat.n_cols; j++) {
            newmat(i,j) = dataset(i+1, j);
        }
    }

    prefix = gaussian ? "_normal_sin_label.txt" : "_uniforme_sin_label.txt";
    file = file + prefix;
    data::Save(file.toStdString().c_str(), newmat, true);
    return 1;
}
