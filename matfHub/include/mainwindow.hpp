#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <stack>
#include "notes.h"


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
    void on_treeView_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_newFileToolbarButton_clicked();

    void on_openFileToolbarButton_clicked();

    void on_saveToolbarButton_clicked();

    void on_copyToolbarButton_clicked();

    void on_pasteToolbarButton_clicked();

    void on_cutToolbarButton_clicked();

    void on_undoToolbarButton_clicked();

    void on_redoToolbarButton_clicked();


private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;

    // Notes
    Notes *notes;
};
#endif // MAINWINDOW_HPP
