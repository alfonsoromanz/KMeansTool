#include "datasetgenerator.h"

DatasetGenerator::DatasetGenerator()
{

}

bool DatasetGenerator::createDataset(const QString &centersFile, long long int pointsPerCluster, long int error, const QString &fileOut)
{
    bool success = 1;
    arma::mat centers;
    success = data::Load(centersFile.toStdString().c_str(), centers, false);

    if (!success) {
        return 0;
    }

    arma::mat dataset;
    int n_clusters = centers.n_cols;
    std::cout << std::endl << "NRO CLUSTERS: " << n_clusters;
    dataset.zeros(centers.n_rows+1, n_clusters * pointsPerCluster);
    std::cout << std::endl << "NRO PUNTOS: " << dataset.n_cols;

    int rows = centers.n_rows+1;
    int cols = n_clusters * pointsPerCluster;
    for (int c=0; c<cols; c++) {
        int cluster = floor(c/pointsPerCluster);
        std::cout << std::endl << "C: " << cluster;
        //set the label
        dataset(0,c)=cluster;
        //generate point
        for (int r=1; r<rows; r++) {
            std::random_device rd;
            std::default_random_engine generator(rd());
            std::uniform_real_distribution<float> distribution(centers(r-1,cluster)-error, centers(r-1,cluster)+error);
            dataset(r, c) = (int) distribution(generator);
        }
    }

    data::Save(fileOut.toStdString().c_str(), dataset, true);
    return 1;
}
