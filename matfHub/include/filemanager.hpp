#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileSystemModel>
#include <stack>

class MainWindow;
class FileManager
{
public:
    FileManager(MainWindow* mw);

    QString oldPath;
    QString appPath;
    QString hubPath;
    QString currPath;

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    int nameSortCounter = 0;
    int dateSortCounter = 0;
    int sizeSortCounter = 0;
    int typeSortCounter = 0;

    int iconSize = 50; //default velicina ikonica

    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;


    void changeDir(const QString path);
    void createNewFolder();

    void dirViewDoubleClicked(const QModelIndex &index);
    void fileViewDoubleClicked(const QModelIndex &index);
    void backButtonClicked();
    void forwardButtonClicked();
    void homeButtonClicked();
    void dotDotButtonClicked();
    void currentFilePathEditingFinished();
    void fileViewCustomContextMenuRequested(const QPoint &pos);
    QString getNameFromIndex(const QModelIndex index);
    void renameSelectedFile(const QModelIndex index, const QString newName);
    void deleteSelectedFiles(const QModelIndexList indices);
    void createNewDocument();
    void sortByName();
    void sortByDate();
    void sortBySize();
    void sortByType();
    void largerIcons();
    void smallerIcons();
    //ako ne gresim pola ovoga treba biti privatno???

private:
    MainWindow *m_mainWindow;

};

#endif // FILEMANAGER_H
