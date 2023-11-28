#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"

#include <QCheckBox>
#include <QString>

Schedule::Schedule(){
}

void Schedule::changeSmer(Ui::MainWindow *ui, int index){
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    for (int i = 0; i < 16; i++) {
        QCheckBox *checkBox = new QCheckBox("Predmet Placeholder" + QString::number(i+1));
        scrollLayout->addWidget(checkBox);
    }
    ui->examArea->setWidget(scrollContent);
};

void Schedule::findSchedule(Ui::MainWindow *ui){
//    logika za nalazenje rasporeda i posele renderovanje u tabelu
}
