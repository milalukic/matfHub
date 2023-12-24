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

    // putanje koje se koriste za navigaciju
    QString oldPath;
    QString appPath;
    QString hubPath;
    QString currPath;

    //modeli koje prikazujemo u levom i desnom delu prozora
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    // pocetne promenljive za sort i reverse sort
    bool m_nameSortCounter = false;
    bool m_dateSortCounter = false;
    bool m_sizeSortCounter = false;
    bool m_typeSortCounter = false;

    // pocetna velicina ikonica
    int m_iconSize = 50;    // Ovde nije bio problem to sto su se lose menjale velicine
        //vec taj da sam hteo da se povecava za +-5 piksela, ali
        //iz nekog razloga QListView, ili sta vec, nije registrovao
        //promene na rasponu od 40-60 pixela.
        //Drugim recima ako bih isao 50+5, ne bi se nista desilo,
        //ali sledeci put kad kliknes da povecas skocilo bi na 60px
        //i ikonice bi se povecale za 10px, sto je bas vidno velika promena,
        //vise izlgeda kao da se dupliraju u velicini nego da se doda 20% na nju
        //ali nakon toga onda radi normalno za +5.
        //Iz tog razloga je bio onaj cudan uslov da prvo poveca/smanji za 10,
        //pa nakon toga ide po 5. U svakom slucaju, slazem se da je bilo ruzno,
        //ali mislim da je to bag na nivou okruzenja (potencijalno mozda samo QListView klase?)
        //tj. ne verujem da mozemo ista da uradimo povodom toga

    // stekovi u kojima cuvamo istoriju kretanja
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;

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
    //ako ne gresim pola ovoga treba biti privatno??? //TODO

private:
    MainWindow *m_mainWindow;

};

#endif // FILEMANAGER_H
