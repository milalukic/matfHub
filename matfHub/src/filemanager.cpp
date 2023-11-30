#include "../include/filemanager.hpp"
#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"

FileManager::FileManager(MainWindow* mw)
    :m_mainWindow(mw)
{

    appPath = QDir::currentPath();

    hubPath = appPath + "/MATF";

    dirModel = new QFileSystemModel();
    dirModel->setRootPath(hubPath);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    fileModel = new QFileSystemModel();
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    fileModel->setReadOnly(false);

    currPath = hubPath;

}

//pravi novi folder u trenutnom direktorijumu desnog pogleda
void FileManager::createNewFolder(){
    QString name = "New Folder";
    QString newFolderPath = currPath;
    newFolderPath.append("/");
    newFolderPath.append(name);
    int i = 1;
    while(QDir(newFolderPath).exists()){
        int foundSpace = name.lastIndexOf("r")+1;
        name = name.left(foundSpace);
        name.append(" ");
        name.append(QString::number(i));
        i++;

        int foundSlash = newFolderPath.lastIndexOf("/");
        newFolderPath = newFolderPath.left(foundSlash+1);
        newFolderPath.append(name);
    }
    QDir(currPath).mkdir(name);
}

//prebacuje desni pogled u zadati direktorijum, postavlja oldPath i currPath, na pozivaocu funkcije je da promenu zabelezi u istoriji navigacije
void FileManager::changeDir(const QString path){
    oldPath = currPath;
    currPath = path;
    m_mainWindow->fileViewSetPath(path);
    m_mainWindow->currentFilePathSetPath(path);
}

//narednih 7 funkcija necu komentarisati, verujem da je jasno sta rade
void FileManager::dirViewDoubleClicked(const QModelIndex &index)
{
    const QString path = dirModel->fileInfo(index).absoluteFilePath();
    changeDir(path);
    navigationBefore.push(oldPath);

}

void FileManager::fileViewDoubleClicked(const QModelIndex &index)
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

void FileManager::backButtonClicked()
{
    if (!navigationBefore.empty()){
        changeDir(navigationBefore.top());
        navigationBefore.pop();
        navigationAfter.push(oldPath);
    }
}

void FileManager::forwardButtonClicked()
{
    if(!navigationAfter.empty()){
        changeDir(navigationAfter.top());
        navigationAfter.pop();
        navigationBefore.push(oldPath);
    }
}

void FileManager::homeButtonClicked()
{
    if(currPath != hubPath){
        changeDir(hubPath);
        navigationBefore.push(oldPath);
    }
}

void FileManager::dotDotButtonClicked()
{
    const int lastSlashFound = currPath.lastIndexOf("/");
    const QString parentDirPath = currPath.left(lastSlashFound);
    if(parentDirPath != currPath){
        changeDir(parentDirPath);
        navigationBefore.push(oldPath);
    }
}

void FileManager::currentFilePathEditingFinished()
{

    QString newPath = m_mainWindow->currentFilePathGetPath();

    //trimujemo kose crte s kraja
    int lastSlashFound = newPath.lastIndexOf("/");
    int lastCharInPath = newPath.length() -1;
    if(lastSlashFound == lastCharInPath){
        while(newPath.at(lastSlashFound--) == '/'){
            newPath.chop(1);
        }
    }

    m_mainWindow->currentFilePathSetPath(newPath);//ovo radimo bez obzira na poziv changeDir da bi se obrisale kose crte ako su one jedine dodate

    if(newPath != currPath){
        const QFileInfo inputFpath(newPath);
        if(inputFpath.exists() && inputFpath.isDir()){
            changeDir(newPath);
            navigationBefore.push(oldPath);
        }
    }
}

void FileManager::fileViewCustomContextMenuRequested(const QPoint &pos)
{
    //TODO
}

QString FileManager::getNameFromIndex(const QModelIndex index){
    return fileModel->fileName(index);
}



void FileManager::renameSelectedFile(const QModelIndex index, const QString newName){
    fileModel->setData(index, newName);
}
void FileManager::deleteSelectedFiles(const QModelIndexList indices){
    for(auto index : indices){
        fileModel->remove(index);
    }
}














