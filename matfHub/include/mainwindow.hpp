#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "schedule.h"
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
    void on_treeView_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_smerBox_activated(int index);

    void on_rasporedStartButton_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;
    Schedule* schedule;
};
#endif // MAINWINDOW_HPP
