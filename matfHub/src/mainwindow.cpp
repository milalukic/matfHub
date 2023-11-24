#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"

#include <QSplitter>
#include <iostream>
#include <QDebug>
#include <string>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    filemanager = new fileManager();

    // ako ne postoji dir MATF, pravimo ga
    if(!QDir("MATF").exists()){
        QDir().mkdir("MATF");
    }

    ui->dirView->setModel(filemanager->dirModel);
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);
    /*/ ui->dirView->setRootIndex(dirModel->setRootPath(hubPath));/**/
    ui->fileView->setModel(filemanager->fileModel);
    ui->fileView->setRootIndex(filemanager->fileModel->setRootPath(filemanager->hubPath));
    ui->currentFilePath->setText(filemanager->currPath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dirView_clicked(const QModelIndex &index)
{

}

//u levom pogledu dupli klik na folder smesta desni pogled u njega
void MainWindow::on_dirView_doubleClicked(const QModelIndex &index)
{
    changeDir(filemanager->dirModel->fileInfo(index).absoluteFilePath(), filemanager);
    filemanager->navigationBefore.push(filemanager->oldPath);
}

//u desnom pogledu dupli klik na stavku vrsi odgovarajucu akciju
void MainWindow::on_fileView_doubleClicked(const QModelIndex &index)
{
    if(filemanager->fileModel->fileInfo(index).isDir()){
        changeDir(filemanager->fileModel->fileInfo(index).absoluteFilePath(), filemanager);
        filemanager->navigationBefore.push(filemanager->oldPath);

    }else{
        //if(podrzan tip fajla){
        //    otvori u odgovarajucem tabu
        //}else{
        QDesktopServices::openUrl(filemanager->fileModel->fileInfo(index).absoluteFilePath());
        //}
    }
}


void MainWindow::on_backButton_clicked()
{
    if (!filemanager->navigationBefore.empty()){
        changeDir(filemanager->navigationBefore.top(), filemanager);
        filemanager->navigationBefore.pop();
        filemanager->navigationAfter.push(filemanager->oldPath);
    }
}

void MainWindow::on_forwardButton_clicked()
{
    if(!filemanager->navigationAfter.empty()){
        changeDir(filemanager->navigationAfter.top(), filemanager);
        filemanager->navigationAfter.pop();
        filemanager->navigationBefore.push(filemanager->oldPath);
    }
}

void MainWindow::on_homeButton_clicked()
{
    if(filemanager->currPath != filemanager->hubPath){
        filemanager->navigationBefore.push(filemanager->currPath);
        ui->fileView->setRootIndex(filemanager->fileModel->setRootPath(filemanager->hubPath));
        filemanager->currPath = filemanager->hubPath;
        ui->currentFilePath->setText(filemanager->currPath);
    }
}

void MainWindow::on_dotDotButton_clicked()
{
    int lastSlashFound = filemanager->currPath.lastIndexOf("/");
    QString parentDirPath = filemanager->currPath.left(lastSlashFound);
    changeDir(parentDirPath, filemanager);
    filemanager->navigationBefore.push(filemanager->oldPath);
}


void MainWindow::on_currentFilePath_editingFinished()
{

    QString newPath = ui->currentFilePath->text();

    //trimujemo poslednje kose crte
    int lastSlashFound = newPath.lastIndexOf("/");
    int lastCharInPath = newPath.length() - 1;
    newPath = newPath.left(lastSlashFound);
    if(lastSlashFound == lastCharInPath){
        while(newPath.at(lastSlashFound--) == '/'){
            newPath.chop(1);
        }//ovo se sigurno moze lepse uraditi?  //uvek moze bolje, samo ne kapiram sta se desava
        //imas one funckije newPath.left(index) i newPath.right(index) koji ti vracaju substring svega na levoj/desnoj strani indeksa (bez indeksa)
        //mozda ti to znaci nesto, necu ja ovo da diram jer ne kapiram ni koje je dugme ni sta treba da uradi
    }
    ui->currentFilePath->setText(newPath);

    if(newPath != filemanager->currPath){

        const QFileInfo inputFpath(newPath);
        if(inputFpath.exists() && inputFpath.isDir()){
            changeDir(ui->currentFilePath->text(), filemanager);
            filemanager->navigationBefore.push(filemanager->oldPath);
        }
    }
}


void MainWindow::on_newFolderButton_clicked() //ovo je za newFolder dugme u UI-u
{
    filemanager->createNewFolder();
}

void MainWindow::on_fileView_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);

    QAction* newFolderAction = new QAction("New Folder", this);
    menu->addAction(newFolderAction);
    connect(newFolderAction, &QAction::triggered, [=]() {//jednakost u prvim zagradama hvata sve promenjive iz pozivajuce funkcije u pozvanu, ne pitaj koja magija niti zasto je to potrebno..
        filemanager->createNewFolder();
    });

    QAction* selectAllAction = new QAction("Select All", this);
    menu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, [=](){
        ui->fileView->clearSelection();
        ui->fileView->selectAll();//ovo ne radi kako sam zamislio i blago je reci da mi ide na zivce :)
        //mozda i radi nego ne prikazuje sve kao plavo, to plavo mozda nije select kako ga mi shvatamo
    });
    //svakako za ovo ispod nam treba neki currentlySelected u kom cemo da cuvamo sta je trenutno selectovano da bi moglo ime da se menja
    //qdir rename menja ime necega u trenutnoj putanji, a mi ne zelimo da ulazimo u nas new folder da bi se on dodao na putanju i onda tek da mozemo da mu menjamo ime
    //pakao brate moj
    QAction* renameAction = new QAction("Rename", this);
    menu->addAction(renameAction);
    connect(renameAction, &QAction::triggered, [=](){
    //    int foundSlash =
    //    QDir::rename();
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

    //o ovome iznad ramisljam vec danima
//    QWidget *wdg = new QWidget;
//    wdg->show();

    QString newHubPath = QFileDialog::getExistingDirectory(this, "Odaberi direktorijum");
    filemanager->hubPath = newHubPath;   //hubPath treba uciniti trajnim nakon zatvaranja programa
    filemanager->currPath = filemanager->hubPath;
    ui->currentFilePath->setText(filemanager->currPath);
    ui->fileView->setRootIndex(filemanager->fileModel->setRootPath(filemanager->currPath));
}

void MainWindow::changeDir(QString path, fileManager *filemanager){
    filemanager->oldPath = filemanager->currPath;

    ui->fileView->setRootIndex(filemanager->fileModel->setRootPath(path));

    filemanager->currPath = path;
    ui->currentFilePath->setText(filemanager->currPath);
}

