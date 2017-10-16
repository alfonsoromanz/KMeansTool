#include "datasetwindow.h"
#include "ui_datasetwindow.h"

DatasetWindow::DatasetWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DatasetWindow)
{
    ui->setupUi(this);
    this->ui->generateButton->setDisabled(true);
    this->ui->progressBar->hide();
}

DatasetWindow::~DatasetWindow()
{
    delete ui;
}

void DatasetWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Cargar Centros"), "./", tr("Archivos de texto (*.txt *.csv)"));
    if (fileName != NULL) {
       this->ui->generateButton->setDisabled(false);
       this->centersDir = getDirectory(fileName);
       this->centersName = getFileName(fileName);
       this->ui->centersLabel->setText(this->centersName);
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

void DatasetWindow::changeCursor(Qt::CursorShape cursor, bool loadingFile)
{
    this->setCursor(cursor);
    this->ui->generateButton->setDisabled(loadingFile);
    qApp->processEvents();
}

void DatasetWindow::on_generateButton_clicked()
{

    this->ui->generateButton->setDisabled(true);
    QString file = this->centersDir + this->centersName;
    QString out = this->centersDir + "dataset_generado";
    long long int pointsPerCluster = this->ui->pointsBox->value();
    long int error = this->ui->errorBox->value();
    bool gaussian = this->ui->comboBox->currentIndex() == 1 ? false : true;

    //Create the Thread
    DatasetGenerator * datasetGenerator = new DatasetGenerator (file, pointsPerCluster, error, gaussian, out, this->ui->progressBar);
    QThread * thread = new QThread;
    datasetGenerator->moveToThread(thread);
    connect (datasetGenerator, SIGNAL (finished(QString, bool)), this, SLOT(handleFinished(QString, bool)));
    connect (thread, SIGNAL(started()), datasetGenerator, SLOT(createDataset()));
    thread->start();

}

void DatasetWindow::handleFinished (QString fileOut, bool success) {
    if (success) {
        QString info = QString("El dataset ha sido almacenado en: ") + fileOut;
        QMessageBox::information(this, "Exito", info);
    } else {
        QMessageBox::warning(this, "ERROR", "Ocurrio un error al cargar el archivo");
    }
    this->ui->generateButton->setDisabled(false);
}

void DatasetWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index==0) {
        this->ui->label_stddev->setText("Varianza");
    } else {
        this->ui->label_stddev->setText("Dispersión en cada eje");
    }
    qApp->processEvents();
}

void DatasetWindow::on_helpButton_clicked()
{
    QString info = QString("\nEl generador de datasets permite crear un dataset a partir de un archivo que contenga los centros. \n\nSe permite la carga de archivos en formato txt o csv con cualquier cantidad de centros y cualquier cantidad de dimensiones.");
    QMessageBox::information(this, "Ayuda", info);
}
