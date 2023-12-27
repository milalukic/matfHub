#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "schedule.h"
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <stack>
#include <memory>
#include <QListView>
#include <QSortFilterProxyModel>
#include "notes.h"


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



private slots:
    void actionExitTriggered();
    void actionChangeHubLocationTriggered();

    //notes slots
    void on_newFileNotesButton_clicked();
    void on_openFileNotesButton_clicked();
    void on_saveNotesButton_clicked();
    void on_copyNotesButton_clicked();
    void on_pasteNotesButton_clicked();
    void on_cutNotesButton_clicked();
    void on_undoNotesButton_clicked();
    void on_redoNotesButton_clicked();

    //fmanager slots
    void dirViewDoubleClicked(const QModelIndex &index);

    void backButtonClicked();
    void forwardButtonClicked();
    void homeButtonClicked();
    void dotDotButtonClicked();
    void newFolderButtonClicked();

    void currentFilePathEditingFinished();

    void fileViewDoubleClicked(const QModelIndex &index);
    void fileViewCustomContextMenuRequested(const QPoint &pos, QAbstractItemView* view);
    void showFileView(QAbstractItemView* view);

    //calculator slots
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

    void on_smerBox_activated(int index);

    void on_rasporedStartButton_clicked();

    void on_scrapeButton_clicked();

private:
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;
    std::unique_ptr<Schedule> schedule;

    int countSelected(const QAbstractItemView* view);
    QModelIndex getSelectedIndex(const QAbstractItemView* view);
    QModelIndexList getSelectedIndices(const QAbstractItemView* view);
    QAbstractItemView* m_activeFileView;

    void setUpFileView(/*enum tipPogleda*/);//funkcija prima enum neki tipa i onda pravi pogled na osnovu enuma i podesava ga, kao sto smo u mejnu radili do sada, zatim brise dete od ui->fileViewLayout i daje mu novo dete, ovo koje je napravio

};
#endif // MAINWINDOW_HPP
