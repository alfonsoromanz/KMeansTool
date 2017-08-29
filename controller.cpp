#include "controller.h"
#include "mainwindow.h"


Controller::Controller(MainWindow * w)
{
    view = w;
}


void Controller::runClustering (const char* dataset, const size_t k, int metric, int maxIterations, bool testingMode) {
    arma::mat data; // Will save all the vectors

    arma::Row<size_t> assignments; // Will save the final assignments

    arma::mat centroids; // Will save the final centroids

    arma::mat auxData;
    arma::Row<size_t> originalAssignments;
    QMap<int, QVector<double>> clusterDictionary; // <Original Cluster, Original Center>
    QVector <int> equivalences; //if equivalences.at(0)=3, cluster 0 is the original cluster 3

    /*
     * Loads dataset in data Matrix
     */
    if (testingMode) {
        //data::Load(dataset, aux, true)
        //copy aux -> data without cluster values
        //also create array with original cluster assignment
    } else {
        data::Load(dataset, data, true);
    }


    /*
     * K-means Model
     */

    if (metric == 1) {
        KMeans<ManhattanDistance> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);
    } else if (metric == 2) {
        KMeans<EuclideanDistance> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);
    } else {
        KMeans<ChebyshevDistance> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);
    }

    /*
     * After Clustering
     */

    if (testingMode) {

        //calculate original centers using data and originalAssignments (Map<cluster, center>)

        //Create equivalences array

        //Create report for each point wrong classified

    }


    // Show Assignments
    for (size_t i = 0; i < assignments.n_elem; ++i)
    {
        std::cout << "El punto " << i << " se asigno al cluster: "
                  << assignments[i] << ".\n";
        view->printMessageLine(QString("El punto " + QString::number(i) + " se asigno al Cluster: " + QString::number(assignments[i])));
    }


    data::Save("assignments_result.csv", assignments, true);
    data::Save("centroids_result.csv", centroids, true);
}
