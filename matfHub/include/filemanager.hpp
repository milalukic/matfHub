#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QObject>
#include <QFileSystemModel>
#include <stack>

class MainWindow;
class FileManager
{
public:
    FileManager(MainWindow* mw);

    // putanje koje se koriste za navigaciju
    QString oldPath;//TODO private
    QString appPath;//vrv private
    QString hubPath;//realno isto private ali mozda geter
    QString currPath;//brate realno private i sigurno geter

    //modeli koje prikazujemo u levom i desnom delu prozora
    QFileSystemModel *dirModel;//private
    QFileSystemModel *fileModel;//private

    // pocetne promenljive za sort i reverse sort
    bool m_nameSortCounter = false;//sve ovo isto private
    bool m_dateSortCounter = false;
    bool m_sizeSortCounter = false;
    bool m_typeSortCounter = false;

    // pocetna velicina ikonica
    int m_iconSize = 50;//priv

    // stekovi u kojima cuvamo istoriju kretanja
    std::stack<QString> navigationBefore;//def priv
    std::stack<QString> navigationAfter;//-||-

    // jasno imenovane funkcije fmanagera
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
    void setSortCounters(bool value);
    void sortByName();
    void sortByDate();
    void sortBySize();
    void sortByType();
    void largerIcons();
    void smallerIcons();

private:
    MainWindow *m_mainWindow;

};

#endif // FILEMANAGER_H
