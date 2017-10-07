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
    this->changeCursor(Qt::WaitCursor, true);
    DatasetGenerator generator;
    QString file = this->centersDir + this->centersName;
    QString out = this->centersDir + "dataset_generado";
    long long int pointsPerCluster = this->ui->pointsBox->value();
    long int error = this->ui->errorBox->value();
    bool gaussian = this->ui->comboBox->currentIndex() == 1 ? false : true;
    bool success = generator.createDataset (file, pointsPerCluster, error, gaussian, out);
    this->changeCursor(Qt::ArrowCursor, false);

    if (success) {
        QString info = QString("El dataset ha sido almacenado en: ") + out;
        QMessageBox::information(this, "Exito", info);
    } else {
        QMessageBox::warning(this, "ERROR", "Ocurrio un error al cargar el archivo");
    }
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
