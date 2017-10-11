#include "clusterer.h"
#include "mainwindow.h"


Clusterer::Clusterer(MainWindow * w)
{
    view = w;
}

void Clusterer::createFiles() {
    arma::imat dataset;

    long int row =32;
    for (int corrida=0; corrida<6; corrida++) {
        dataset.zeros(row, 2);
        for (long int j=0; j<dataset.n_cols; j++) {
            for (long int i=0; i<dataset.n_rows; i++) {
                if (j==0) {
                    dataset(i,j) = 500;
                } else {
                    if (i==0) {
                        dataset(i,j) = 600;
                    } else {
                        dataset(i,j) = 510;
                    }
                }
            }
        }
        QString name = QString("dataset_") + QString::number(row) + QString(".txt");
        data::Save(name.toStdString().c_str(), dataset, true);
        row=row*4;
    }

}

void Clusterer::runClustering (arma::mat &dataset, const size_t k, int metric, int maxIterations, const QString &directory, const QString &file, bool testingMode) {
    view->clear();
    view->printMessageLine("Ejecutando clustering...");

    auto begin = std::chrono::high_resolution_clock::now();
    arma::mat data(dataset); // Will save all the vectors

    arma::Row<size_t> assignments; // Will save the final assignments

    arma::mat centroids; // Will save the final centroids


    arma::Row<size_t> originalAssignments;


    QString assignmentsPath = directory + QString("asignaciones_clustering.txt");
    QString centroidsPath = directory + QString("centros_calculados.txt");
    QString originalCentroidsPath = directory + QString("centros_originales.txt");
    QString reportPath = directory + QString("reporte_clustering.txt");

    long int pointsPerCluster[k];
    for (int i=0; i<k; i++) {
        pointsPerCluster[i]=0;
    }

    QMap<int, arma::Col<double>> * clusterDictionary = NULL;

    /*
     * Loads dataset in data Matrix
     */
    if (testingMode) {

        //Asume que la primer columna es el cluster
        data.zeros(dataset.n_rows-1, dataset.n_cols);

        //copy aux -> data without cluster values
        for (size_t r=1; r<dataset.n_rows; ++r)
            for (size_t c=0; c<dataset.n_cols; ++c) {
                data(r-1,c)=dataset(r,c);
            }


        //also create array with original cluster assignment
        originalAssignments.zeros(dataset.n_cols);
        size_t cluster_row = 0; //asume la primera fila
        for (size_t col=0; col<dataset.n_cols; col++) {
            originalAssignments[col] = dataset(cluster_row,col);
        }


        //calculate original centers using data and originalAssignments (Map<cluster, center>)
        clusterDictionary = calculateMeans(data, originalAssignments);

        if (clusterDictionary->keys().size() != k) {
            int difference = abs(clusterDictionary->keys().size()-k);
            std::string error;
            if (difference > 20) {
                error = std::string("Para ejecutar un Test de Clustering, el dataset debe contener las etiquetas en la primer columna. \n\nSi el archivo ya contiene las etiquetas, revise el Nro. de Clusters especificado.");
            } else {
                error = std::string("Nro. de clusters incorrecto. Para el dataset especificado debe agrupar en ") + std::to_string(clusterDictionary->keys().size()) + std::string(" clusters.");
            }
            QMessageBox messageBox;
            messageBox.critical(0, "Error", error.c_str());
            messageBox.setFixedSize(500,200);
            this->view->clear();
            return;
        }

    }

    //data::Save("means.txt", getMean(data,5,9), true);

    /*
     * K-means Model
     */
    std::string metricName;
    auto begin1 = std::chrono::high_resolution_clock::now();
    if (metric == 1) {

        metricName = "Manhattan";
        KMeans<ManhattanDistance, RefinedStart, MaxVarianceNewCluster, NaiveKMeans, arma::mat> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);

    } else if (metric == 0) {

        metricName = "Euclidean";
        KMeans<EuclideanDistance, RefinedStart, MaxVarianceNewCluster, NaiveKMeans, arma::mat> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);
    } else {

        metricName = "Chebyshev";
        KMeans<ChebyshevDistance, RefinedStart, MaxVarianceNewCluster, NaiveKMeans, arma::mat> k_means(maxIterations);
        k_means.Cluster(data, k, assignments, centroids);
    }
    auto end2 = std::chrono::high_resolution_clock::now();

    this->view->clear();
    this->view->printMessageLine(QString("\nClustering Finalizado. Tiempo: " + QString::number(std::chrono::duration_cast<std::chrono::nanoseconds>(end2-begin1).count()) + " ns  (" + QString::number(std::chrono::duration_cast<std::chrono::seconds>(end2-begin1).count()) + " s)."));

    //Save assignments
    this->view->printMessageLine(QString(QString("\nGuardando asignaciones en: ") + assignmentsPath));
    data::Save(assignmentsPath.toStdString(), assignments, true);

    //Save centroids
    this->view->printMessageLine(QString(QString("\nGuardando centroides en: ") + centroidsPath));
    data::Save(centroidsPath.toStdString(), centroids, true);




    if (testingMode) {

        //Save Original centroids
        this->view->printMessageLine(QString(QString("\nGuardando centros originales en: ") + originalCentroidsPath));
        this->saveToFile(originalCentroidsPath.toStdString(), clusterDictionary);
    }


    /*
     * After Clustering: Report
     */

    this->view->printMessageLine(QString("\nGenerando reporte..."));

    //Create report for each point wrong classified
    int wrongPoints = 0;
    QList<QString> reportedPoints;

    if (testingMode) {

        QMap<int,int> equivalences(this->clusterEquivalences(*clusterDictionary, centroids, metric));

        for (size_t col=0; col < data.n_cols; col++) {
            if (originalAssignments[col]!= equivalences.value(assignments[col])) {
                wrongPoints++;
                QString message = "\tEl punto '" + QString::number(col) + "' : \n";
                message += "\n\t\t"+ pointToString(data, col) + "\n\n";
                message += "\t\tHa sido clasificado al cluster '" + QString::number(assignments[col]) + "', con centro en: \n";
                message += "\n\t\t\t"+ pointToString(centroids, assignments[col]) + "\n\n";
                message += "\t\tY debia ser clasificado al cluster '" + QString::number(originalAssignments[col]) + "', con centro en: \n";
                message += "\n\t\t\t"+ colToString(clusterDictionary->value(originalAssignments[col])) + "\n";
                reportedPoints.push_back(message);
            }
        }

    }

    for (int i=0; i<assignments.n_elem; i++) {
        pointsPerCluster[assignments[i]]++;
    }

    int totalPoints = assignments.n_elem;
    double accuracy = double(double(totalPoints-wrongPoints) / double(totalPoints));
    std::ofstream report;
    report.open(reportPath.toStdString().c_str());
    report << "\n Resultados del clustering";
    report << "\n *************************";
    report << "\n\n Dataset: " + QString(directory+file).toStdString();
    report << "\n Metrica utilizada: " << metricName;
    report << "\n Tiempo total: " << QString::number(std::chrono::duration_cast<std::chrono::nanoseconds>(end2-begin1).count()).toStdString() << " ns";
    report << "\n Asignaciones: ";
    for (int i=0; i<k; i++) {
        report << "\n   Cluster " << i << ": " << pointsPerCluster[i] << " puntos";
    }
    report << "\n Total de puntos: " << totalPoints;
    if (testingMode) {
        report << "\n Precision: " << std::fixed <<std::setprecision(5) << double(accuracy*100) <<"%";
        report << "\n Puntos mal clasificados: " << wrongPoints << "\n\n";
        if (wrongPoints>0) {
            for (int i=0; i<reportedPoints.size(); i++) {
                report << std::endl << reportedPoints.at(i).toStdString();
            }
        }
    }
   report << std::endl;

   report.close();
   this->view->printMessageLine(QString(QString("\nReporte almacenado en: ") + QString(reportPath)));

    auto end = std::chrono::high_resolution_clock::now();

}

QMap<int,arma::Col<double>> * Clusterer::calculateMeans(arma::mat data, arma::Row<size_t> originalAssignments)
{
    QMap <int, size_t> points; //points per cluster
    QMap<int,arma::Col<double>> * result = new QMap<int,arma::Col<double>>();
    for (size_t c=0; c < data.n_cols; c++) {
        int cluster = originalAssignments.at(c);
        if (result->contains(cluster)) {
            arma::Col<double> col (result->value(cluster));
            result->remove(cluster);
            int size = points.value(cluster);
            points.remove(cluster);
            points.insert(cluster, size+1);
            col = col + data.col(c);
            result->insert(cluster, col);
        }else {
            points.insert(cluster, 1);
            arma::Col<double> newMean;
            newMean.zeros(data.n_rows);
            newMean = newMean + data.col(c);
            result->insert(cluster, newMean);
        }
    }

    QList<int> keys(result->keys());
    for (int i=0; i<keys.size(); i++) {
        int cluster = keys.at(i);
        arma::Col<double> col (result->value(cluster));
        result->remove(cluster);
        col = col / points.value(cluster);
        result->insert(cluster, col);
    }

    return result;

}

arma::Row<double> Clusterer::getMean(arma::mat data,size_t startCol, size_t endCol)
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



QString Clusterer::pointToString(arma::mat &data, size_t pointColumn)
{
    QString result;
    result = "";
    for (size_t row=0; row < data.n_rows; row++) {
        result += QString::number(data(row, pointColumn)) + "\t";
    }
    return result;
}

QString Clusterer::colToString(const arma::Col<double> &data)
{
    QString result;
    result = "";
    for (size_t row=0; row < data.n_rows; row++) {
        result += QString::number(data.at(row)) + "\t";
    }
    return result;
}

QMap<int, int> Clusterer::clusterEquivalences(const QMap<int, arma::Col<double>> &originalCentroids, const arma::mat &newCentroids, int metric)
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

double Clusterer::getDistance(const arma::Col<double> &pointA, const arma::Col<double> &pointB, int metric)
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

void Clusterer::saveToFile(const std::string &path, QMap<int, arma::Col<double> > *dictionary)
{
    QList<int> keys (dictionary->keys());
    std::ofstream report;
    report.open(path);
    report << "# Centros originales: ";
    report << std::endl;
    for (size_t i=0; i<keys.size(); i++) {
        int cluster = keys.at(i);
        arma::Col<double> centroid (dictionary->value(cluster));
        report << std::endl;
        report << "# Cluster: " << cluster << std::endl;
        report << colToString(centroid).toStdString();
        report << std::endl;
    }


   report.close();
}
