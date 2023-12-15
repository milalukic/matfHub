#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/course.h"

#include <QCheckBox>
#include <QString>
#include <string>
#include <iostream>
#include <set>

Schedule::Schedule(){
    kmiljanScraper = std::make_unique<KmiljanScraper>();
}

void Schedule::changeModule(Ui::MainWindow *ui, int index){
//    if(index == 0) {
//        return;
//    }
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    std::string module = modules[index];
    std::set<std::string> courseSet;
    for(Course course: moduleCourseMap[module]) {
        courseSet.insert(course.description);
    }
    for(auto course: courseSet){
        QCheckBox *checkBox = new QCheckBox(QString::fromStdString(course));
        scrollLayout->addWidget(checkBox);
    }
    delete ui->examArea->takeWidget(); // fix memory leak
    ui->examArea->setWidget(scrollContent);
};

void Schedule::findSchedule(Ui::MainWindow *ui){
//    logika za nalazenje rasporeda i posele renderovanje u tabelu
}

void Schedule::downloadSchedule(Ui::MainWindow *ui){
    moduleCourseMap = kmiljanScraper->download();
    modules.clear();
    for (const auto& pair : moduleCourseMap) {
        modules.push_back(pair.first);
    }
    ui->smerBox->setPlaceholderText("Izaberi smer");
    ui->smerBox->clear();
    for(auto& module : modules){
        ui->smerBox->addItem(QString::fromStdString(module));
    }
};

void Schedule::scrapeSchedule(Ui::MainWindow *ui){
    QString oldText = ui->scrapeButton->text();
    ui->scrapeButton->setText("Scraping...");
    downloadSchedule(ui);
    ui->scrapeButton->setText(oldText);
    ui->rasporedStartButton->setEnabled(true);
    ui->scheduleTable->setEnabled(true);
    ui->smerBox->setEnabled(true);
    ui->examArea->setEnabled(true);
}
