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
}

void MainWindow::printMessageLine(const QString & message) {
    printMessage("\n" + message);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setController(Controller * c) {
    controller = c;
    controller->runClustering("g2.txt", 2, 1, 100, false);

}
