#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "history.hpp"
#include "schedule.h"
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <stack>
#include <memory>
#include <QListView>
#include <QString>

#include <QSortFilterProxyModel>

#include "notes.h"
#include "calendar.h"

#include "plotter.hpp"
#include "matrix.hpp"
#include "statistics.hpp"

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
    void currentFilePathSetPath(const QString path);// v
    QString currentFilePathGetPath();//znam, ruzno ime... currentFilePath je lajn edit koji bi trebalo u svakom momentu da sadrzi putanju u kojoj se nalazi menadzer
    Notes *notes;
    FileManager* m_fileManager;
    Ui::MainWindow *ui;



    //TODO PITAJ ***********************************************************************
    Plotter *plt;


private slots:
    //notes slots
    void on_newFileToolbarButton_clicked();
    void on_openFileToolbarButton_clicked();
    void on_saveToolbarButton_clicked();
    void on_copyToolbarButton_clicked();
    void on_pasteToolbarButton_clicked();
    void on_cutToolbarButton_clicked();
    void on_undoToolbarButton_clicked();
    void on_redoToolbarButton_clicked();
    //fmanager slots
    void on_dirView_doubleClicked(const QModelIndex &index);
    void on_backButton_clicked();
    void on_forwardButton_clicked();
    void on_homeButton_clicked();
    void fileViewDoubleClicked(const QModelIndex &index);
    void on_dirView_clicked(const QModelIndex &index);
    void on_currentFilePath_editingFinished();
    void on_dotDotButton_clicked();
    void on_newFolderButton_clicked();
    void on_fileView_customContextMenuRequested(const QPoint &pos);
    void on_actionExit_triggered();

    void on_actionDark_Mode_triggered();

    void on_actionChangeHubLocation_triggered();
    //calculator slots
//    void pbMatrixTest();
    void changeStackedWidgetPage();

        //calc functions
    void writeToHistoryTB(History* history);
            //regular
    void calculateRegular();
            //matrix
    void showMatrix(Matrix *m);

    void calculateMatrixTranspose();
    void calculateMatrixInverse();
    void calculateMatrixDiag();
    void calculateMatrixOne();

    QStringList matrixStringToStringList(QString str);
    void reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos, QStringList content);
    void reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos);
    void reshapeMatrix1();
    void reshapeMatrix2();
    QString readM1Data();
    QString readM2Data();

    void calculateMatrixAdd();
    void calculateMatrixSubtract();
    void calculateMatrixMultiply();
    void calculateMatrixDivide();

    void saveMatrix();
    void loadMatrix(unsigned int pos, QStringList strLst, unsigned d1, unsigned d2);

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
    void plotParse();

    void plotLinspace();
    void savePlotting();

    //Staistics
    void statCalcMean();
    void statCalcVariance();
    void statCalcStd();
    void statCalcMedian();
    void statCalcMode();
    void statPlotHist();
    void statPlotBar();
    void statPlotBox();


    void historySave();

    //kmiljanify
    void on_smerBox_activated(int index);

    void on_rasporedStartButton_clicked();

    void on_scrapeButton_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;
    std::unique_ptr<Schedule> schedule;

    Calendar *calendar;

    int countSelected(const QTableView* view);
    QModelIndex getSelectedIndex(const QTableView* view);
    QModelIndexList getSelectedIndices(const QTableView* view);

    void setUpFileView(/*enum tipPogleda*/);//funkcija prima enum neki tipa i onda pravi pogled na osnovu enuma i podesava ga, kao sto smo u mejnu radili do sada, zatim brise dete od ui->fileViewLayout i daje mu novo dete, ovo koje je napravio

};
#endif // MAINWINDOW_HPP
