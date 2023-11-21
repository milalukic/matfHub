#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <stack>
#include <iostream>
#include <QDebug>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    appPath = QDir::currentPath();

    // ako ne postoji dir MATF, pravimo ga
    if(!QDir("MATF").exists()){
        QDir().mkdir("MATF");
    }
    hubPath = appPath + "/MATF";

    dirModel = new QFileSystemModel(this);
/**/dirModel->setRootPath(hubPath);/**/
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->dirView->setModel(dirModel);
/*/ ui->dirView->setRootIndex(dirModel->setRootPath(hubPath));/**/
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->fileView->setModel(fileModel);
    ui->fileView->setRootIndex(fileModel->setRootPath(hubPath));

    currPath = hubPath;

    ui->currentFilePath->setText(currPath);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dirView_clicked(const QModelIndex &index)
{
    //ceo ovaj slot nam samo pravi probleme

//    if(ui->dirView->isExpanded(index)){
//        ui->dirView->collapse(index);
//    }else{
//        ui->dirView->expand(index);
//    }
}

void MainWindow::on_dirView_doubleClicked(const QModelIndex &index)
{//nervira me mnogo on ga i klikne i dabl klikne pa se iz prethodne funkcije i otvori i zatvori ne znam kako da ga sprecim

    //ui->dirView->expand(index);  //ovo je podrazumevana akcija, a ako je expanded onda collapsuje

    changeDir(dirModel->fileInfo(index).absoluteFilePath());
    navigationBefore.push(oldPath);
}


void MainWindow::on_fileView_doubleClicked(const QModelIndex &index)
{

    if(fileModel->fileInfo(index).isDir()){
        changeDir(fileModel->fileInfo(index).absoluteFilePath());
        navigationBefore.push(oldPath);

    }else{
        //if(podrzan tip fajla){
        //    otvori u odgovarajucem tabu
        //}else{
        QDesktopServices::openUrl(fileModel->fileInfo(index).absoluteFilePath());
        //}
    }

}


void MainWindow::on_backButton_clicked()
{
    if (!navigationBefore.empty()){
        changeDir(navigationBefore.top());
        navigationBefore.pop();
        navigationAfter.push(oldPath);
    }
}

void MainWindow::on_forwardButton_clicked()
{
    if(!navigationAfter.empty()){
        changeDir(navigationAfter.top());
        navigationAfter.pop();
        navigationBefore.push(oldPath);
    }
}

void MainWindow::on_homeButton_clicked()
{
    if(currPath != hubPath){
        navigationBefore.push(currPath);
        ui->fileView->setRootIndex(fileModel->setRootPath(hubPath));
        currPath = hubPath;
        ui->currentFilePath->setText(currPath);
    }
}

void MainWindow::on_dotDotButton_clicked()
{
    int found = currPath.lastIndexOf("/");
    if(found != -1){
        QString parentDirPath = currPath.left(found);
        changeDir(parentDirPath);
        navigationBefore.push(oldPath);
    }
}


void MainWindow::on_currentFilePath_editingFinished()
{
    const QString newPath = ui->currentFilePath->text();
    if(newPath != currPath){

        const QFileInfo inputFpath(newPath);

        if(inputFpath.exists() && inputFpath.isDir()){
            changeDir(ui->currentFilePath->text());
            navigationBefore.push(oldPath);
        }
    }
}

void MainWindow::changeDir(QString path){
    oldPath = currPath;
    newPath = path;

    ui->fileView->setRootIndex(fileModel->setRootPath(newPath));

    currPath = newPath;
    ui->currentFilePath->setText(currPath);
}

void MainWindow::on_pushButton_clicked() //ovo je za newFolder dugme u UI-u
{
    //ovo trenutno samo pravi new folder, treba dodati da korisnik moze da ukuca ime foldera
    // postoji funkcija path.rename("ime") koja na nas path tipa nestonesto/NewFolder taj new folder
    // promeni u ime, a path promeni u nestonesto/ime
    // to treba da se napravi da bude interaktivno doduse

    //nesto u fazonu kad se klikne newfolder dugme da se pojavi novi folder sa ponudjenom opcijum a upises ime foldera
    // ili ako ostavis prazno da samo napise New Folder (broj) sto kod ispod vec radi
    QString name = "New Folder";
    QString path = currPath;
    path.append("/");
    path.append(name);
    int i = 1;
    while(QDir(path).exists()){
        int foundSpace = name.lastIndexOf("r");
        name = name.left(foundSpace+1);
        name.append(" ");
        name.append(QString::number(i));
        i++;

        int foundSlash = path.lastIndexOf("/");
        path = path.left(foundSlash+1);
        path.append(name);
    }
    QDir(currPath).mkdir(name);
}
