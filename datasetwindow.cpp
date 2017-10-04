#include "datasetwindow.h"
#include "ui_datasetwindow.h"

DatasetWindow::DatasetWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DatasetWindow)
{
    ui->setupUi(this);
    this->ui->generateButton->setDisabled(true);
}

DatasetWindow::~DatasetWindow()
{
    delete ui;
}

void DatasetWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Cargar Centros"), "./", tr("Archivos de texto (*.txt *.csv)"));
    if (fileName != NULL) {
       this->ui->centersLabel->setText(fileName);
       this->ui->generateButton->setDisabled(false);
       this->centersDir = getDirectory(fileName);
       this->centersName = getFileName(fileName);
    }
}

QString DatasetWindow::getDirectory(const QString &filePath) {
    QString ret = filePath;
    ret.remove(this->getFileName(filePath));
    return ret;
}


QString DatasetWindow::getFileName(const QString &filePath)
{
    QFileInfo i(filePath);
    QString ret = i.fileName();
    return ret;
}

void DatasetWindow::on_generateButton_clicked()
{
    DatasetGenerator generator;
    QString file = this->centersDir + this->centersName;
    QString out = this->centersDir + "dataset_generado.txt";
    long long int pointsPerCluster = this->ui->pointsBox->value();
    long int error = this->ui->errorBox->value();
    generator.createDataset (file, pointsPerCluster, error, out);
}
