#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileSystemModel>
#include <stack>


class fileManager
{
public:
    fileManager();
    QString oldPath;
    QString appPath;
    QString hubPath;
    QString currPath;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;

    void createNewFolder();

private:


};

#endif // FILEMANAGER_H
