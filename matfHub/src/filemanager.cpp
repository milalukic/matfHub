#include "../include/filemanager.hpp"
#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"

fileManager::fileManager()
{

    appPath = QDir::currentPath();

    hubPath = appPath + "/MATF";

    dirModel = new QFileSystemModel();
    dirModel->setRootPath(hubPath);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    fileModel = new QFileSystemModel();
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    currPath = hubPath;

}

//prima novu putanju, na nju menja trenutnu putanju u desnom pogledu i lajn editu, podesava staru i novu putanju glavnog prozora
//na pozivacu je da staru putanju smesti u odgovarajuci stek istorije

void fileManager::createNewFolder(){
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
