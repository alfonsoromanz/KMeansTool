#include "mainwindow.h"
#include <QApplication>
#include <mlpack/core.hpp>
#include <cmath>
#include <mlpack/methods/kmeans/kmeans.hpp>


using namespace mlpack;
using namespace mlpack::kmeans;
using namespace mlpack::metric;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    arma::mat data; // Alojara el Dataset

        arma::Row<size_t> assignments; // Tendra las asignaciones finales

        arma::mat centroids; // Tendra los centroides finales

        /*
         * Carga el dataset especificado en la matriz "data"
         */
        data::Load("g2.txt", data, true);


        /*
         * Parametros del algoritmo
         */

        int max_iterations = 100;
        size_t k = 2;


        /*
         * Modelo k-means
         */

        KMeans<ChebyshevDistance> k_means(max_iterations);

        k_means.Cluster(data, k, assignments, centroids);


        // Muestra asignaciones
        for (size_t i = 0; i < assignments.n_elem; ++i)
        {
            std::cout << "El punto " << i << " se asigno al cluster: "
            << assignments[i] << ".\n";
        }

        // Muestra los centroides
        /*
        for (size_t i = 0; i < centroids.n_cols; ++i) {
          std::cout << "Centroide " << i << " es: (";
          for (size_t j = 0; j < centroids.n_rows; ++j) {
              std::cout << centroids[i][j] << ", ";
          }
          std::cout << ".\n";
        }*/

        data::Save("assignments_result.csv", assignments, true);
        data::Save("centroids_result.csv", centroids, true);


    return a.exec();
}
