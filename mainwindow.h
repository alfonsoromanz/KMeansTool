#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include <QFile>
#include <QtWidgets>
#include <QApplication>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setController (Controller * c);
    void printMessage (const QString & message);
    void printMessageLine(const QString & message);
    void clear();
    ~MainWindow();

private slots:
    void on_loadButton_clicked();

    void on_runButton_clicked();

private:
    Ui::MainWindow *ui;
    Controller * controller;
};

#endif // MAINWINDOW_H
