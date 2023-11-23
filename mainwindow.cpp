#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <stack>
#include <iostream>
#include <QDebug>
#include <string>
#include <QFileDialog>

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

    //model za levi prikaz, samo folderi i vreme poslednje izmene
    dirModel = new QFileSystemModel(this);
/**/dirModel->setRootPath(hubPath);/**/
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    //vezujemo model za pogled
    ui->dirView->setModel(dirModel);
/*/ ui->dirView->setRootIndex(dirModel->setRootPath(hubPath));/**/
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);

    //model za desni prikaz, folderi i fajlovi
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    //vezujemo model za pogled
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

//u levom pogledu dupli klik na folder smesta desni pogled u njega
void MainWindow::on_dirView_doubleClicked(const QModelIndex &index)
{//nervira me mnogo on ga i klikne i dabl klikne pa se iz prethodne funkcije i otvori i zatvori ne znam kako da ga sprecim
    //ui->dirView->expand(index);  //ovo je podrazumevana akcija, a ako je expanded onda collapsuje

    changeDir(dirModel->fileInfo(index).absoluteFilePath());
    navigationBefore.push(oldPath);
}

//u desnom pogledu dupli klik na stavku vrsi odgovarajucu akciju
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
    int lastSlashFound = currPath.lastIndexOf("/");
    QString parentDirPath = currPath.left(lastSlashFound);
    changeDir(parentDirPath);
    navigationBefore.push(oldPath);
}


void MainWindow::on_currentFilePath_editingFinished()
{

    QString newPath = ui->currentFilePath->text();

    //trimujemo poslednje kose crte
    int lastSlashFound = newPath.lastIndexOf("/");
    int lastCharInPath = newPath.length() - 1;
    if(lastSlashFound == lastCharInPath){
        while(newPath.at(lastSlashFound--) == '/'){
            newPath.chop(1);
        }//ovo se sigurno moze lepse uraditi?
    }
    ui->currentFilePath->setText(newPath);

    if(newPath != currPath){

        const QFileInfo inputFpath(newPath);
        if(inputFpath.exists() && inputFpath.isDir()){
            changeDir(ui->currentFilePath->text());
            navigationBefore.push(oldPath);
        }
    }
}

//prima novu putanju, na nju menja trenutnu putanju u desnom pogledu i lajn editu, podesava staru i novu putanju glavnog prozora
//na pozivacu je da staru putanju smesti u odgovarajuci stek istorije
void MainWindow::changeDir(QString path){
    oldPath = currPath;
    newPath = path; //ja mislim da je ovo polje klase nepotrebno, imamo curr koji je bitan i old koji se gura u stek a new koristi samo ova f-ja ako ne gresim, ekvivalentan je sa lokalnim argumentom 'path'

    ui->fileView->setRootIndex(fileModel->setRootPath(newPath));

    currPath = newPath;
    ui->currentFilePath->setText(currPath);
}

void MainWindow::on_newFolderButton_clicked() //ovo je za newFolder dugme u UI-u
{
    //ovo trenutno samo pravi new folder, treba dodati da korisnik moze da ukuca ime foldera
    // postoji funkcija path.rename("ime") koja na nas path tipa nestonesto/NewFolder taj new folder
    // promeni u ime, a path promeni u nestonesto/ime
    // to treba da se napravi da bude interaktivno doduse

    //nesto u fazonu kad se klikne newfolder dugme da se pojavi novi folder sa ponudjenom opcijum a upises ime foldera
    // ili ako ostavis prazno da samo napise New Folder (broj) sto kod ispod vec radi
    createNewFolder();
}

void MainWindow::createNewFolder()
{
    QString name = "New Folder";
    QString newFolderPath = currPath;
    newFolderPath.append("/");
    newFolderPath.append(name);
    int i = 1;
    while(QDir(newFolderPath).exists()){
        int foundSpace = name.lastIndexOf("r");
        name = name.left(foundSpace+1);
        name.append(" ");
        name.append(QString::number(i));
        i++;

        int foundSlash = newFolderPath.lastIndexOf("/");
        newFolderPath = newFolderPath.left(foundSlash+1);
        newFolderPath.append(name);
    }
    QDir(currPath).mkdir(name);
}


void MainWindow::on_fileView_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);

    QAction* newFolderAction = new QAction("New Folder", this);
    menu->addAction(newFolderAction);
    connect(newFolderAction, &QAction::triggered, [=]() {//jednakost u prvim zagradama hvata sve promenjive iz pozivajuce funkcije u pozvanu, ne pitaj koja magija niti zasto je to potrebno..
        createNewFolder();
    });

    QAction* selectAllAction = new QAction("Select All", this);
    menu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, [=](){
        ui->fileView->clearSelection();
        ui->fileView->selectAll();//ovo ne radi kako sam zamislio i blago je reci da mi ide na zivce :)
    });


    menu->popup(ui->fileView->viewport()->mapToGlobal(pos));


}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionChangeHubLocation_triggered()
{
    //ovde treba otvoriti fajl eksplorer u novom prozoru u kome bi se birao novi hubPath
    //sada bi bio <del>pravi</del> poslednji trenutak da fMenadzer izdvojimo kao svoju klasu iz glavnog prozora jer nam je potreban 1) u tabu fMngr, 2) pri menjanju hubPath, 3) pri odabiru Save As, 4) itd itd
//    QWidget *wdg = new QWidget;
//    wdg->show();

    QString newHubPath = QFileDialog::getExistingDirectory(this, "Odaberi direktorijum");
    hubPath = newHubPath;   //hubPath treba uciniti trajnim nakon zatvaranja programa
    currPath = hubPath;
    ui->currentFilePath->setText(currPath);
    ui->fileView->setRootIndex(fileModel->setRootPath(currPath));
}

