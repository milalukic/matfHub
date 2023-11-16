#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString sPath = ""; //ovde kasnije dodati path i gurnuti ga u setRootPath
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(QDir::rootPath());
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(dirModel);

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());
    fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    ui->listView->setModel(fileModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(sPath));
}


void MainWindow::on_pushButton_clicked()
{
    //Ovde za back
}


void MainWindow::on_pushButton_2_clicked()
{
    //Ovde za forward
}


void MainWindow::on_pushButton_3_clicked()
{
    //Ovde za return to home
}

