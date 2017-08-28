#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->plainTextEdit->insertPlainText(QString("KMeansTool by Alfonso Román Zubeldia"));
    this->ui->plainTextEdit->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setController(Controller * c) {
    controller = c;
    controller->runClustering("g2.txt", 2, 1, 100, false);

}
