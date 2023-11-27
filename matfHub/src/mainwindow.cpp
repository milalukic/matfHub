#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <stack>
// #include <iostream>
// #include "../include/notes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    notes = new class notes();
    QString sPath = ""; //ovde kasnije dodati path i gurnuti ga u setRootPath
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(QDir::rootPath());
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(dirModel);

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());
    fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    ui->listView->setModel(fileModel);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(sPath));

    navigationBefore.push(dirModel->fileInfo(index).absoluteFilePath());
}


void MainWindow::on_pushButton_clicked()
{
    if (!navigationBefore.empty()){
        QString sBack = navigationBefore.top();
        navigationBefore.pop();
        fileModel->setRootPath(sBack);
        dirModel->setRootPath(sBack);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    //Ovde za forward
}


void MainWindow::on_pushButton_3_clicked()
{
    //Ovde za return to home
}


void MainWindow::on_pushButton_5_clicked()
{
    notes->currentFile.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_pushButton_6_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Otvori novu datoteku");
    QFile file(fileName);
    notes->currentFile = fileName;

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        return;
    }

    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();

    ui->textEdit->setText(text);
    file.close();
}


void MainWindow::on_pushButton_8_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Sacuvaj.");
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        return;
    }

    notes->currentFile = fileName;
    setWindowTitle(fileName);

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_pushButton_7_clicked()
{
    ui->textEdit->copy();
}


void MainWindow::on_pushButton_11_clicked()
{
    ui->textEdit->paste();
}


void MainWindow::on_pushButton_9_clicked()
{
    ui->textEdit->cut();
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->textEdit->undo();
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit->redo();
}



