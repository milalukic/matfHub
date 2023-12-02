#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./../parser/parser.cpp"
#include <cstring>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pbParser, &QPushButton::clicked, this, &MainWindow::calculateResult);
    connect(ui->pbChangeRegular, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangeMatrix, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangePlotting, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeStackedWidgetPage(int d){

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

void MainWindow::calculateResult(){

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
