#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <stack>
// #include <iostream>
// #include "../include/notes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    notes = new class Notes();
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
    //je l ovde ide "close without saving?"?
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(sPath));

    navigationBefore.push(dirModel->fileInfo(index).absoluteFilePath());
}


void MainWindow::on_pushButton_clicked()
{
    if (!navigationBefore.empty()){
        QString sBack = navigationBefore.top();
        navigationBefore.pop();
        fileModel->setRootPath(sBack);
        dirModel->setRootPath(sBack);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    //Ovde za forward
}


void MainWindow::on_pushButton_3_clicked()
{
    //Ovde za return to home
}


// Funkcionalnosti Notes toolbara
void MainWindow::on_newFileToolbarButton_clicked()
{
    notes->newClicked(ui);
}

void MainWindow::on_openFileToolbarButton_clicked()
{
    notes->openClicked(ui, this);
}

void MainWindow::on_saveToolbarButton_clicked()//save/save as? trenutno najlaksa opcija da se sacuva izmena jednog fajla u drugi je ctrl+a ctrl+c ctrl+n ctrl+v ctrl+s (takodje bilo bi kul da se prva tri dugmeta aktiviraju i na ctrl+n ctrl+s i ctrl+o
{
    notes->saveClicked(ui, this);
}

void MainWindow::on_copyToolbarButton_clicked()
{
    notes->copyClicked(ui);
}

void MainWindow::on_pasteToolbarButton_clicked()
{
    notes->pasteClicked(ui);
}

void MainWindow::on_cutToolbarButton_clicked()
{
    notes->cutClicked(ui);
}

void MainWindow::on_undoToolbarButton_clicked()
{
    notes->undoClicked(ui);
}

void MainWindow::on_redoToolbarButton_clicked()
{
    notes->redoClicked(ui);
}
