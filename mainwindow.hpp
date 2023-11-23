#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_dirView_doubleClicked(const QModelIndex &index);

    void on_backButton_clicked();

    void on_forwardButton_clicked();

    void on_homeButton_clicked();

    void on_fileView_doubleClicked(const QModelIndex &index);

    void on_dirView_clicked(const QModelIndex &index);

    void on_currentFilePath_editingFinished();

    void on_dotDotButton_clicked();

    void on_newFolderButton_clicked();

    void createNewFolder();

    void on_fileView_customContextMenuRequested(const QPoint &pos);

    void on_actionExit_triggered();


    void on_actionChangeHubLocation_triggered();

private:
    Ui::MainWindow *ui;
    QString oldPath;
    QString newPath; //ja mislim da je ovo polje klase nepotrebno, imamo curr koji je bitan i old koji se gura u stek a new koristi samo f-ja changeDir ako ne gresim, ekvivalentan je sa njenim lokalnim argumentom 'path'
    QString appPath;
    QString hubPath;
    QString currPath;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;

    void changeDir(QString path);
};
#endif // MAINWINDOW_HPP
