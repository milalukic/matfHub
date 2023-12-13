#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "schedule.h"
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <stack>
#include <memory>
#include <QListView>

#include "notes.h"
#include "plotter.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FileManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fileViewSetPath(const QString path);
    void currentFilePathSetPath(const QString path);
    QString currentFilePathGetPath();

    //TODO PITAJ ***********************************************************************
    Plotter *plt;


private slots:
    void on_newFileToolbarButton_clicked();

    void on_openFileToolbarButton_clicked();

    void on_saveToolbarButton_clicked();

    void on_copyToolbarButton_clicked();

    void on_pasteToolbarButton_clicked();

    void on_cutToolbarButton_clicked();

    void on_undoToolbarButton_clicked();

    void on_redoToolbarButton_clicked();

    void on_dirView_doubleClicked(const QModelIndex &index);

    void on_backButton_clicked();

    void on_forwardButton_clicked();

    void on_homeButton_clicked();

    void on_fileView_doubleClicked(const QModelIndex &index);

    void on_dirView_clicked(const QModelIndex &index);

    void on_currentFilePath_editingFinished();

    void on_dotDotButton_clicked();

    void on_newFolderButton_clicked();

    void on_fileView_customContextMenuRequested(const QPoint &pos);

    void on_actionExit_triggered();

    void on_actionChangeHubLocation_triggered();

    //calculator things
//    void pbMatrixTest();
    void changeStackedWidgetPage();

        //calc functions
            //regular
    void calculateRegular();
            //matrix
    void calculateMatrixTranspose();
    void calculateMatrixInverse();
    void calculateMatrixDiag();
    void calculateMatrixOne();

    void parseMatrix1();
    void parseMatrix2();

    void calculateMatrixAdd();
    void calculateMatrixSubtract();
    void calculateMatrixMultiply();
    void calculateMatrixDivide();

        //plotting
    void plot();
    void plotSin();
    void plotCos();
    void plotTan();
    void plotLn();
    void plotLog();
    void plotExp();
    void plotAbs();
    void plotNeg();
    void plotSquare();
    void plotRoot();
    void plotLinspace();

    //Staistics
    void statCalcMean();
    void statCalcVariance();
    void statCalcMedian();
    void statCalcMode();
    void statPlotHist();
    void statPlotBar();


    //kmiljanify
    void on_smerBox_activated(int index);

    void on_rasporedStartButton_clicked();

    void on_scrapeButton_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;
    std::unique_ptr<Schedule> schedule;

    Notes *notes;

    FileManager* m_fileManager;
    int countSelected(const QListView* view);
    QModelIndex getSelectedIndex(const QListView* view);
    QModelIndexList getSelectedIndices(const QListView* view);

};
#endif // MAINWINDOW_HPP
