#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->printMessage(QString("KMeansTool"));
    this->printMessageLine(QString("By Alfonso Román Zubeldia"));
    this->ui->plainTextEdit->setReadOnly(true);
}

void MainWindow::printMessage(const QString & message) {
    this->ui->plainTextEdit->insertPlainText(message);
    qApp->processEvents();
}

void MainWindow::printMessageLine(const QString & message) {
    printMessage("\n" + message);
    qApp->processEvents();
}

void MainWindow::clear() {
    this->ui->plainTextEdit->clear();
    qApp->processEvents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setController(Controller * c) {
    controller = c;


}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Dataset"), "./", tr("Text Files (*.txt *.csv)"));
    if (fileName != NULL) {
        if (this->processDataset(fileName)) {
           this->ui->datasetLabel->setText(fileName);
           this->datasetDir = this->getDirectory(fileName);
           this->datasetName = this->getFileName(fileName);
           this->datasetReady = true;
        }
    }
}

void MainWindow::on_runButton_clicked()
{
    if (datasetReady) {
        controller->runClustering(datasetMatrix, 2, 1, 100, this->datasetDir, this->datasetName, true);
    }
}

bool MainWindow::processDataset(const QString &dataset)
{
    data::Load(dataset.toStdString().c_str(), this->datasetMatrix, true);
    return 1;
}

QString MainWindow::getDirectory(const QString &filePath) {
    QString ret = filePath;
    ret.remove(this->getFileName(filePath));
    return ret;
}


QString MainWindow::getFileName(const QString &filePath)
{
    QFileInfo i(filePath);
    QString ret = i.fileName();
    return ret;
}
