#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileSystemModel>
#include <stack>
#include <QSortFilterProxyModel>

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
    QSortFilterProxyModel *proxyModel;

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
    //ako ne gresim pola ovoga treba biti privatno???

private:
    MainWindow *m_mainWindow;

};

#endif // FILEMANAGER_H
