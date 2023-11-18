#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./../parser/parser.cpp"
#include <cstring>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pbParser, &QPushButton::clicked, this, &MainWindow::onClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClick(){

    Parser *parser = new Parser();

    //TODO so it works with char*
    char *text = ui->leParser->text().toStdString().data();
    parser->eval_exp(text);
    double res = parser->eval_exp(text);
    QString qres = QString::number(res);

    ui->leParser->setText(qres);
    ui->tbParser->setText(qres);

    delete parser; parser = nullptr;
}
