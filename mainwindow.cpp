#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(loadAction);
    menu.addAction(createDataAction);
    menu.setTitle("&Archivo");
    menu.exec(event->globalPos());

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->plainTextEdit->setReadOnly(true);
    this->ui->runButton->setDisabled(true);
    this->createActions();
    this->createMenu();
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

void MainWindow::setClusterer(Clusterer * c) {
    clusterer = c;
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Cargar Dataset"), "./", tr("Archivos de texto (*.txt *.csv)"));
    if (fileName != NULL) {
        if (this->processDataset(fileName)) {
           this->datasetDir = this->getDirectory(fileName);
           this->datasetName = this->getFileName(fileName);
           this->ui->datasetLabel->setText(this->datasetName);
           this->datasetReady = true;
           this->ui->runButton->setDisabled(false);
        }
    }
}

void MainWindow::on_runButton_clicked()
{
    if (datasetReady) {
        int k = this->ui->clustersBox->value();
        int max_iterations = this->ui->maxIterationsBox->value();
        int metric = this->ui->metricsBox->currentIndex();
        bool testingMode = this->ui->modeBox->currentIndex();
        clusterer->runClustering(datasetMatrix, k, metric, max_iterations, this->datasetDir, this->datasetName, testingMode);
    }
}

void MainWindow::create_dataset_clicked()
{
    DatasetWindow * window = new DatasetWindow();
    window->show();
}

bool MainWindow::processDataset(const QString &dataset)
{
    bool success = 1;
    this->changeCursor(Qt::WaitCursor, true);
    success = data::Load(dataset.toStdString().c_str(), this->datasetMatrix, false);
    this->changeCursor(Qt::ArrowCursor, false);
    if (!success) {
        this->ui->datasetLabel->setText("Ningun dataset cargado");
        this->datasetDir ="";
        this->datasetName="";
        this->datasetReady = false;
        this->ui->runButton->setDisabled(true);
        QMessageBox::warning(this, "ERROR", "Ocurrio un error al cargar el dataset");
        qApp->processEvents();
    }

    return success;
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

void MainWindow::changeCursor(Qt::CursorShape cursor, bool loadingFile)
{
    this->setCursor(cursor);
    this->ui->loadButton->setDisabled(loadingFile);
    qApp->processEvents();
}

void MainWindow::createActions()
{
    loadAction = new QAction(tr("&Cargar Dataset"), this);
    loadAction->setStatusTip(tr("Carga un dataset para ejecutar K-Means"));
    connect(loadAction, &QAction::triggered, this, &MainWindow::on_loadButton_clicked);

    createDataAction = new QAction(tr("&Crear Dataset"), this);
    createDataAction->setStatusTip(tr("Crea un nuevo dataset"));
    connect (createDataAction, &QAction::triggered, this, &MainWindow::create_dataset_clicked);
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&Archivo"));
    fileMenu->addAction(loadAction);
    fileMenu->addAction(createDataAction);

    button = new QToolButton();
    button->setText("Archivo  ");
    button->setMenu(fileMenu);
    button->setPopupMode(QToolButton::InstantPopup);
    this->ui->mainToolBar->addWidget(button);
}

void MainWindow::on_helpButton_clicked()
{
    QString info = QString("\nEl modo de ejecución \"Descubrir Clusters\" permite ejecutar el algoritmo K-Means sobre bases de datos sin etiquetas de cluster (con la distribución de los puntos a su cluster desconocida). \n\nEl modo de ejecución \"Test de Clustering\" sólo está disponible para aquellas bases de datos que contengan la etiqueta de asignación de cada punto a su cluster (En la primer columna). Luego del agrupamiento, se evaluarán las asignaciones y se generará un reporte con los puntos mal clasificados.\n");
    QMessageBox::information(this, "Ayuda", info);
}
