#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    ui->treeView ->setModel(model);

    model->setRootPath(QDir::rootPath());
    model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->tableView ->setModel(model);
    QObject::connect(ui->tableView, &QTableView::doubleClicked, ui->tableView, &QTableView::setRootIndex);
    ui->tableView->show();


}

MainWindow::~MainWindow()
{
    delete ui;
}

