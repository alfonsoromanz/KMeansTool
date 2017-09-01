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
    QMap<int, arma::Row<size_t>> clusterDictionary; // <Original Cluster, Original Center>
    QVector <int> equivalences; //if equivalences.at(0)=3, cluster 0 is the original cluster 3

    /*
     * Loads dataset in data Matrix
     */
    if (testingMode) {
        //Asume que la primer columna es el cluster
        data::Load(dataset, auxData, true);
        data.zeros(auxData.n_rows-1, auxData.n_cols);

        //copy aux -> data without cluster values
        for (size_t r=1; r<auxData.n_rows; ++r)
            for (size_t c=0; c<auxData.n_cols; ++c) {
                data(r-1,c)=auxData(r,c);
            }

        //also create array with original cluster assignment
        originalAssignments.zeros(auxData.n_cols);
        size_t cluster_row = 0; //asume la primera fila
        for (size_t col=0; col<auxData.n_cols; col++) {
            originalAssignments[col] = auxData(cluster_row,col);
        }


        data::Save("originales.txt", originalAssignments, true);
        data::Save("preprocesada.txt", data, true);


    } else {
        data::Load(dataset, data, true);
    }

    //data::Save("means.txt", getMean(data,5,9), true);

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
        QMap<int, arma::Col<double>> means (calculateMeans(data, originalAssignments));
        /*
        QList<int> keys(means.keys());
        for (int i=0; i<keys.size(); i++) {
            std::cout << "Cluster: " << keys.at(i) << " --> MEDIA: " << means.value(keys.at(i)) << ".\n";
        }*/

        //Create equivalences array

        //Create report for each point wrong classified

    }


    // Show Assignments
    /*
    for (size_t i = 0; i < assignments.n_elem; ++i)
    {
        std::cout << "El punto " << i << " se asigno al cluster: "
                  << assignments[i] << ".\n";
        view->printMessageLine(QString("El punto " + QString::number(i) + " se asigno al Cluster: " + QString::number(assignments[i])));
    }*/


    data::Save("assignments_result.csv", assignments, true);
    data::Save("centroids_result.csv", centroids, true);
}

QMap<int,arma::Col<double>> Controller::calculateMeans(arma::mat data, arma::Row<size_t> originalAssignments)
{
    QMap <int, size_t> points; //points per cluster
    QMap<int,arma::Col<double>> result;
    for (size_t c=0; c < data.n_cols; c++) {
        int cluster = originalAssignments.at(c);
        if (result.contains(cluster)) {
            arma::Col<double> col (result.value(cluster));
            result.remove(cluster);
            int size = points.value(cluster);
            points.remove(cluster);
            points.insert(cluster, size+1);
            col = col + data.col(c);
            result.insert(cluster, col);
        }else {
            points.insert(cluster, 1);
            arma::Col<double> newMean;
            newMean.zeros(data.n_rows);
            newMean = newMean + data.col(c);
            result.insert(cluster, newMean);
        }
    }

    QList<int> keys(result.keys());
    for (int i=0; i<keys.size(); i++) {
        int cluster = keys.at(i);
        arma::Col<double> col (result.value(cluster));
        result.remove(cluster);
        col = col / points.value(cluster);
        result.insert(cluster, col);
    }

    return result;

}

arma::Row<double> Controller::getMean(arma::mat data,size_t startCol, size_t endCol)
{
    arma::Row<double> result;
    size_t dim = data.n_rows;
    size_t n = endCol - startCol + 1;
    result.zeros(dim);

    //calculate the sum
    for (size_t col=startCol; col<=endCol; col++) {
        for (size_t i=0; i<dim; i++) {
            result[i] += data(i,col);
        }
    }

    //divide by n
    for (size_t i=0; i<dim; i++) {
        result[i] = result[i] / n;
    }
    return result;
}

size_t Controller::getEndOfSequence(arma::Row<size_t> pointsVector, size_t start)
{
    if (start<0 || start >= pointsVector.n_rows) {
        return -1;
    }

    int cluster = pointsVector.at(start);

}
