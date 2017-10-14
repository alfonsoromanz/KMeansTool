#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clusterer.h"
#include "datasetwindow.h"
#include <QFile>
#include <QtWidgets>
#include <QApplication>
#include "datasetgenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    #ifndef QT_NO_CONTEXTMENU
        void contextMenuEvent(QContextMenuEvent *event) override;
    #endif // QT_NO_CONTEXTMENU

public:
    explicit MainWindow(QWidget *parent = 0);
    void setClusterer (Clusterer * c);
    void printMessage (const QString & message);
    void printMessageLine(const QString & message);
    void clear();
    ~MainWindow();

private slots:
    void on_loadButton_clicked();
    void on_runButton_clicked();
    void create_dataset_clicked();

    void on_helpButton_clicked();

    void handleResult(QString message, bool success);

private:
    Ui::MainWindow *ui;
    Clusterer * clusterer;
    QMenu * fileMenu;
    QAction * loadAction;
    QAction * createDataAction;
    QToolButton * button;

    bool datasetReady = false;
    QString datasetDir = "";
    QString datasetName ="";
    arma::mat datasetMatrix;

    bool processDataset (const QString &dataset);
    QString getDirectory (const QString & filePath);
    QString getFileName (const QString & filePath);

    void changeCursor(Qt::CursorShape cursor, bool loadingFile);

    void createActions();
    void createMenu();
};

#endif // MAINWINDOW_H
