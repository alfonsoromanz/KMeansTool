#include "controller.h"
#include "mainwindow.h"


Controller::Controller(MainWindow * w)
{
    view = w;
}


void Controller::runClustering (const char* dataset, const size_t k, int metric, int maxIterations, bool testingMode) {
    view->clear();
    view->printMessageLine("Ejecutando clustering...");

    auto begin = std::chrono::high_resolution_clock::now();
    arma::mat data; // Will save all the vectors

    arma::Row<size_t> assignments; // Will save the final assignments

    arma::mat centroids; // Will save the final centroids

    arma::mat auxData;
    arma::Row<size_t> originalAssignments;

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


        //data::Save("originales.txt", originalAssignments, true);
        //data::Save("preprocesada.txt", data, true);


    } else {
        data::Load(dataset, data, true);
    }

    //data::Save("means.txt", getMean(data,5,9), true);

    /*
     * K-means Model
     */
    std::string metricName;
    auto begin1 = std::chrono::high_resolution_clock::now();
    if (metric == 1) {

        metricName = "Manhattan";
        KMeans<ManhattanDistance> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);

    } else if (metric == 0) {

        metricName = "Euclidean";
        KMeans<EuclideanDistance> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);
    } else {

        metricName = "Chebyshev";
        KMeans<ChebyshevDistance> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);
    }
    auto end2 = std::chrono::high_resolution_clock::now();

    this->view->printMessageLine(QString("\nClustering Finalizado (Tiempo: " + QString::number(std::chrono::duration_cast<std::chrono::nanoseconds>(end2-begin1).count()) + " ns)."));


    /*
     * After Clustering
     */

    if (testingMode) {
        this->view->printMessageLine(QString("\nGenerando reporte..."));

        //calculate original centers using data and originalAssignments (Map<cluster, center>)
        QMap<int, arma::Col<double>> clusterDictionary (calculateMeans(data, originalAssignments));
        /*
        QList<int> keys(means.keys());
        for (int i=0; i<keys.size(); i++) {
            std::cout << "Cluster: " << keys.at(i) << " --> MEDIA: " << means.value(keys.at(i)) << ".\n";
        }*/

        //Create report for each point wrong classified
        int wrongPoints = 0;
        QList<QString> reportedPoints;

        QMap<int,int> equivalences(this->clusterEquivalences(clusterDictionary, centroids, metric));
        for (size_t col=0; col < data.n_cols; col++) {
            if (originalAssignments[col]!= equivalences.value(assignments[col])) {
                wrongPoints++;
                QString message = "\tEl punto '" + QString::number(col) + "' : \n";
                message += "\n\t\t"+ pointToString(data, col) + "\n\n";
                message += "\t\tHa sido clasificado al cluster '" + QString::number(assignments[col]) + "', con centro en: \n";
                message += "\n\t\t\t"+ pointToString(centroids, assignments[col]) + "\n\n";
                message += "\t\tY debia ser clasificado al cluster '" + QString::number(originalAssignments[col]) + "', con centro en: \n";
                message += "\n\t\t\t"+ colToString(clusterDictionary.value(originalAssignments[col])) + "\n";
                reportedPoints.push_back(message);
            }
        }

        std::ofstream report;
        report.open("report.txt");
        report << "\n Resultados del clustering";
        report << "\n *************************";
        report << "\n\n Dataset: " + std::string(dataset);
        report << "\n Metrica utilizada: " << metricName;
        report << "\n Puntos mal clasificados: " << wrongPoints << "\n\n";
        if (wrongPoints>0) {
            for (int i=0; i<reportedPoints.size(); i++) {
                report << std::endl << reportedPoints.at(i).toStdString();
            }
        }

        report << std::endl;

       report.close();
       this->view->printMessageLine(QString("\nReporte almacenado en report.txt"));





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

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::endl << "TIEMPO : "<< std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << " s" << std::endl;


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



QString Controller::pointToString(arma::mat &data, size_t pointColumn)
{
    QString result;
    result = "";
    for (size_t row=0; row < data.n_rows; row++) {
        result += QString::number(data(row, pointColumn)) + "\t";
    }
    return result;
}

QString Controller::colToString(const arma::Col<double> &data)
{
    QString result;
    result = "";
    for (size_t row=0; row < data.n_rows; row++) {
        result += QString::number(data.at(row)) + "\t";
    }
    return result;
}

QMap<int, int> Controller::clusterEquivalences(const QMap<int, arma::Col<double>> &originalCentroids, const arma::mat &newCentroids, int metric)
{
    QMap<int,int> result;
    QList<int> keys(originalCentroids.keys());
    for (size_t col=0; col<newCentroids.n_cols; col++) {
        arma::Col<double> centroid = newCentroids.col(col);
        double minDistance=99999;
        size_t nearest = 0;
        for (size_t i=0; i<keys.size(); i++) {
            double dist = getDistance(centroid, originalCentroids.value(keys.at(i)), metric);
            if (dist<minDistance) {
                minDistance=dist;
                nearest = keys.at(i);
            }
        }
        result.insert(col, nearest);
    }


    return result;
}

double Controller::getDistance(const arma::Col<double> &pointA, const arma::Col<double> &pointB, int metric)
{
    if (metric==1) {
        ManhattanDistance metric;
        return metric.Evaluate(pointA, pointB);
    } else if (metric ==0) {
        EuclideanDistance metric;
        return metric.Evaluate(pointA, pointB);
    } else if (metric==2) {
        ChebyshevDistance metric;
        return metric.Evaluate(pointA, pointB);
    }
    return 0;
}
