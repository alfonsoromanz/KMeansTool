#ifndef DATASETWINDOW_H
#define DATASETWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include "datasetgenerator.h"

namespace Ui {
class DatasetWindow;
}

class DatasetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DatasetWindow(QWidget *parent = 0);
    ~DatasetWindow();

private slots:
    void on_pushButton_clicked();

    void on_generateButton_clicked();

private:
    Ui::DatasetWindow *ui;

    QString centersDir = "";
    QString centersName ="";

    QString getDirectory (const QString & filePath);
    QString getFileName (const QString & filePath);
};

#endif // DATASETWINDOW_H
