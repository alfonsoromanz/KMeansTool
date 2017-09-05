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
    QString fileName = QFileDialog::getOpenFileName(this);
    //this->datasetLabel.setText(fileName);
    std::cout << std::endl << "FILE : " << fileName.toStdString();
}

void MainWindow::on_runButton_clicked()
{
    controller->runClustering("g2_known.txt", 2, 1, 100, true);
}
