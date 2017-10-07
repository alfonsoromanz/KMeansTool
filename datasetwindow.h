#ifndef DATASETWINDOW_H
#define DATASETWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include "datasetgenerator.h"
#include <QMessageBox>

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

    void on_comboBox_currentIndexChanged(int index);

    void on_helpButton_clicked();

private:
    Ui::DatasetWindow *ui;

    QString centersDir = "";
    QString centersName ="";

    QString getDirectory (const QString & filePath);
    QString getFileName (const QString & filePath);
    void changeCursor(Qt::CursorShape cursor, bool loadingFile);
};

#endif // DATASETWINDOW_H
