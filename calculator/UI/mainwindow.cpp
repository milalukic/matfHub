#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./../parser/parser.cpp"
//#include "./../src/matrix/matrix.cpp"
#include <cstring>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pbParser, &QPushButton::clicked, this, &MainWindow::calculateRegular);

    connect(ui->pbChangeRegular, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangeMatrix, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangePlotting, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);

    connect(ui->pbMatrixTranspose, &QPushButton::clicked, this, &MainWindow::calculateMatrixTranspose);
    connect(ui->pbMatrixInverse, &QPushButton::clicked, this, &MainWindow::calculateMatrixInverse);
    connect(ui->pbMatrixDiag, &QPushButton::clicked, this, &MainWindow::calculateMatrixDiag);
    connect(ui->pbMatrixOne, &QPushButton::clicked, this, &MainWindow::calculateMatrixOne);

    connect(ui->pbMatrixLoad1, &QPushButton::clicked, this, &MainWindow::parseMatrix1);
    connect(ui->pbMatrixLoad2, &QPushButton::clicked, this, &MainWindow::parseMatrix2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeStackedWidgetPage(){

    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonText = buttonSender->text();
    std::cout << buttonText.toStdString() << std::endl;

    if(buttonText == "Regular calculator")
        ui->stackedWidget->setCurrentIndex(0);
    if(buttonText == "Matrix calculator")
        ui->stackedWidget->setCurrentIndex(1);
    if(buttonText == "Plotting calculator")
        ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::calculateRegular(){

    Parser *parser = new Parser();

    //TODO so it works with char*
    char *expr = ui->leParser->text().toStdString().data();
    parser->eval_exp(expr);
    double res = parser->eval_exp(expr);
    QString qres = QString::number(res);

    QString history = ui->tbParser->toPlainText();

    history += expr;
    history += "\n";
    history += qres;
    history += "\n--------------\n";

    ui->leParser->setText(qres);
    ui->tbParser->setText(history);

    delete parser; parser = nullptr;
}

void MainWindow::calculateMatrixTranspose(){

    //TODO make this work (CMake probably)
//    Matrix *m1 = new Matrix(5, 5);
    std::cout << "Transpose" << std::endl;
}

void MainWindow::calculateMatrixInverse(){
    std::cout << "Inverse" << std::endl;
}

void MainWindow::calculateMatrixDiag(){
    std::cout << "Diag" << std::endl;
}

void MainWindow::calculateMatrixOne(){
    std::cout << "One" << std::endl;
}

void MainWindow::parseMatrix1(){
    QString text = "Dim1: " + ui->leMatrixDim11->text() + " Dim2: " + ui->leMatrixDim12->text() + " data: " + ui->leMatrixData1->text();
    std::cout << text.toStdString() << std::endl;

}

void MainWindow::parseMatrix2(){
    QString text = "Dim1: " + ui->leMatrixDim21->text() + " Dim2: " + ui->leMatrixDim22->text() + " data: " + ui->leMatrixData2->text();
    std::cout << text.toStdString() << std::endl;
}

