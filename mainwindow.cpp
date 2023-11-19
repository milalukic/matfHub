#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <stack>
#include <iostream>
#include <QDebug>

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

    QString oldPath = currPath;
    QString newPath = dirModel->fileInfo(index).absoluteFilePath();

    ui->fileView->setRootIndex(fileModel->setRootPath(newPath));
    navigationBefore.push(oldPath);

    currPath = newPath;
    ui->currentFilePath->setText(currPath); //Bojane molim te smisli kako ovo pametnije da se uradi bas ne mogu sad da razmisljam, hvala puno :*
    //mozda nesto tipa nmp da imamo klasu singlton CurrPath gde se na currPath.set(QString newPath) pored promene polja objketa takodje menja i sadrzaj labele u guiu koju ova klasa vidi iz nekog razloga itd

}


void MainWindow::on_fileView_doubleClicked(const QModelIndex &index)
{

    if(fileModel->fileInfo(index).isDir()){
        QString oldPath = currPath;
        QString newPath = fileModel->fileInfo(index).absoluteFilePath();

        ui->fileView->setRootIndex(fileModel->setRootPath(newPath));
        navigationBefore.push(oldPath);

        currPath = newPath;
        ui->currentFilePath->setText(currPath);
        //ovo je bukvalno kopiran kod iz on_dirView_doubleClicked, vrv treba izdvojiti u funkciju tipa sutra ili tako nekad
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
        QString newPath = navigationBefore.top();
        QString oldPath = currPath;
        navigationBefore.pop();
        ui->fileView->setRootIndex(fileModel->setRootPath(newPath));
        navigationAfter.push(oldPath);
        currPath = newPath;
        ui->currentFilePath->setText(currPath);//opet ovo :)
    }
}

void MainWindow::on_forwardButton_clicked()
{
    if(!navigationAfter.empty()){
        QString newPath = navigationAfter.top();
        QString oldPath = currPath;
        navigationAfter.pop();
        ui->fileView->setRootIndex(fileModel->setRootPath(newPath));
        navigationBefore.push(oldPath);
        currPath = newPath;
        ui->currentFilePath->setText(currPath);// (:
    }
}

void MainWindow::on_homeButton_clicked()
{
    if(currPath != hubPath){
        navigationBefore.push(currPath);
        ui->fileView->setRootIndex(fileModel->setRootPath(hubPath));
        currPath = hubPath;
        ui->currentFilePath->setText(currPath); // zadavicu se crevom od tusa ovo je pretnja
    }
}

void MainWindow::on_dotDotButton_clicked()
{
    if(!currPath.isEmpty() && currPath != QString::fromStdString("/")){//vrv treba uraditi da se pri menjanju trenutne putanje uvek trimuje poslednji slesh jer mogu zamisliti da ovo vrlo lako i brzo postane nepodnosljiva kombinatorna eksplozija provera a da mi se radilo sa stringovima do kraja zivota ne bih polozio p2
        //QDir::cdUp radi nesto pametno
        //zapravo generalno QDir mislim da bi hahaha bilo pametno da vise koristimo al ok
        QDir dir(currPath);
        dir.cdUp();
        const QString oldPath = currPath;
        currPath = dir.absolutePath();
        ui->fileView->setRootIndex(fileModel->setRootPath(currPath));
        ui->currentFilePath->setText(currPath);
        navigationBefore.push(oldPath);
        //a sta mi tesko da kopiram isti kod deset puta nije cak ni kopiran nego prekucan dobicu histericni napad
    }
}


void MainWindow::on_currentFilePath_editingFinished()
{
    const QString newPath = ui->currentFilePath->text();
    if(newPath != currPath){

        const QFileInfo inputFpath(newPath);

        if(inputFpath.exists() && inputFpath.isDir()){
            QString oldPath = currPath;
            QString newPath = ui->currentFilePath->text();

            ui->fileView->setRootIndex(fileModel->setRootPath(newPath));
            navigationBefore.push(oldPath);

            currPath = newPath;
            ui->currentFilePath->setText(currPath);
            //zaboga miloga opet ovaj kod kopiran covek je coveku najveci neprijatelj a ja sam sebi
        }
    }
}



