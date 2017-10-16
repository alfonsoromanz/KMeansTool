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
    this->ui->progressBar->hide();
    this->ui->datasetProgressBar->hide();
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
        this->loadDataset(fileName);
    }
}

void MainWindow::on_runButton_clicked()
{
    if (datasetReady) {
        int k = this->ui->clustersBox->value();
        int max_iterations = this->ui->maxIterationsBox->value();
        int metric = this->ui->metricsBox->currentIndex();
        bool testingMode = this->ui->modeBox->currentIndex();


        Clusterer * clusterer = new Clusterer(datasetMatrix, k, metric, max_iterations, this->datasetDir, this->datasetName, this->ui->progressBar ,testingMode);
        QThread * thread = new QThread;
        clusterer->moveToThread(thread);
        connect (clusterer, SIGNAL (finished(QString,bool)), this, SLOT(handleResult(QString, bool)));
        connect (thread, SIGNAL(started()), clusterer, SLOT(runClustering()));
        this->ui->runButton->setDisabled(true);
        this->clear();
        this->printMessageLine("Ejecutando Clustering...");

        thread->start();
    }
}

void MainWindow::create_dataset_clicked()
{
    DatasetWindow * window = new DatasetWindow();
    window->show();
}

void MainWindow::loadDataset(const QString &dataset)
{
    this->ui->datasetLabel->setText("Cargando dataset...");
    this->ui->loadButton->setDisabled(true);
    qApp->processEvents();
    DatasetLoader * datasetLoader = new DatasetLoader (dataset, &datasetMatrix, this->ui->datasetProgressBar);
    QThread * thread = new QThread;
    datasetLoader->moveToThread(thread);
    connect (datasetLoader, SIGNAL (finished(QString, QString, bool)), this, SLOT(handleDataset(QString, QString, bool)));
    connect (thread, SIGNAL(started()), datasetLoader, SLOT(loadDataset()));
    thread->start();
    /*
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

    return success;*/
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

void MainWindow::handleResult(QString message, bool success)
{
    this->clear();
    if (!success) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", message.toStdString().c_str());
        messageBox.setFixedSize(500,200);
    }
    this->printMessageLine(message);
    this->ui->runButton->setDisabled(false);
    qApp->processEvents();
}

void MainWindow::handleDataset(QString datasetDir, QString datasetName, bool success)
{
    this->datasetDir = datasetDir;
    this->datasetName= datasetName;
    this->datasetReady = success;
    this->ui->runButton->setDisabled(!success);

    if (success) {
        this->ui->datasetLabel->setText(this->datasetName);
    } else {
        this->ui->datasetLabel->setText("Ningún dataset cargado");
        QMessageBox::warning(this, "ERROR", "Ocurrio un error al cargar el dataset");
    }
    this->ui->loadButton->setDisabled(false);
    qApp->processEvents();
}
