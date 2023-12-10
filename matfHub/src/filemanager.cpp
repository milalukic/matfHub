#include "../include/filemanager.hpp"
#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/config.hpp"

FileManager::FileManager(MainWindow* mw)
    :m_mainWindow(mw)
{

    appPath = QDir::currentPath();

    hubPath = appPath + "/" + Config::getConfig()->getHubPath();

    dirModel = new QFileSystemModel();
    dirModel->setRootPath(hubPath);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    fileModel = new QFileSystemModel();
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    fileModel->setReadOnly(false);

    currPath = hubPath;
}

//pravi novi folder/dokument u trenutnom direktorijumu desnog pogleda
void FileManager::createNewFolder(){
    QString name = "New Folder";
    QString newFolderPath = currPath;
    newFolderPath.append("/");
    newFolderPath.append(name);
    int i = 1;
    while(QDir(newFolderPath).exists()){
        int foundSpace = name.lastIndexOf("r")+1; //osecam se kao da je ovo r magicna konstanta nz treba li je promeniti
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
void FileManager::createNewDocument(){
    QString name = "New Document.txt";
    QString newFilePath = currPath;
    newFilePath.append("/");
    newFilePath.append(name);
    int i = 1;
    while(QFile(newFilePath).exists()){
        int foundSpace = name.indexOf("t")+1;
        name = name.left(foundSpace);
        name.append(" ");
        name.append(QString::number(i));
        i++;
        name.append(".txt");

        int foundSlash = newFilePath.lastIndexOf("/");
        newFilePath = newFilePath.left(foundSlash+1);
        newFilePath.append(name);
    }
    QFile file(newFilePath);
    file.open(QIODevice::ReadWrite | QFile::Text);
    file.close();
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
        QString clickedFilePath = fileModel->fileInfo(index).absoluteFilePath();
        auto lastDot = clickedFilePath.lastIndexOf(".");
        if(lastDot != -1){
            auto fileExtension = clickedFilePath.last(clickedFilePath.length() - lastDot);
            if(".txt" == fileExtension || ".json" == fileExtension){
                m_mainWindow->ui->tabWidgetMatfHub->setCurrentIndex(2); //ovo je tvrdo kodirano, bilo bi lepo izvuci indeks iz imena ili makar definisati enum
                m_mainWindow->notes->openFile(clickedFilePath, m_mainWindow->ui, m_mainWindow);
            }else{
                QDesktopServices::openUrl(fileModel->fileInfo(index).absoluteFilePath());
            }
        }

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


void FileManager::sortByName(){
    //qDebug() << "Column 0:" << fileModel->headerData(0, Qt::Horizontal, Qt::DisplayRole);
    if (nameSortCounter%2==1){
        fileModel->sort(0, Qt::AscendingOrder);
        nameSortCounter++;
    }else{
        fileModel->sort(0, Qt::DescendingOrder);
        nameSortCounter++;
    }
}
void FileManager::sortByDate(){
    //qDebug() << "Column 3:" << fileModel->headerData(3, Qt::Horizontal, Qt::DisplayRole);
    if (dateSortCounter%2==1){
        fileModel->sort(3, Qt::AscendingOrder);
        dateSortCounter++;
    }else{
        fileModel->sort(3, Qt::DescendingOrder);
        dateSortCounter++;
    }
}
void FileManager::sortBySize(){
    //qDebug() << "Column 1:" << fileModel->headerData(1, Qt::Horizontal, Qt::DisplayRole);
    if (sizeSortCounter%2==1){
        fileModel->sort(1, Qt::AscendingOrder);
        sizeSortCounter++;
    }else{
        fileModel->sort(1, Qt::DescendingOrder);
        sizeSortCounter++;
    }
}
void FileManager::sortByType(){
    //qDebug() << "Column 2:" << fileModel->headerData(2, Qt::Horizontal, Qt::DisplayRole);
    if (typeSortCounter%2==1){
        fileModel->sort(2, Qt::AscendingOrder);
        typeSortCounter++;
    }else{
        fileModel->sort(2, Qt::DescendingOrder);
        typeSortCounter++;
    }
}











