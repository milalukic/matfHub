#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/course.h"

#include <QCheckBox>
#include <QString>
#include <string>

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
    for(Course course: moduleCourseMap[module]) {
        QCheckBox *checkBox = new QCheckBox(QString::fromStdString(course.description));
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
    ui->smerBox->setPlaceholderText("Izaberi smer");
    ui->smerBox->clear();
    for(auto& module : modules){
        ui->smerBox->addItem(QString::fromStdString(module));
    }
};

void Schedule::scrapeSchedule(Ui::MainWindow *ui){
    QString oldText = ui->scrapeButton->text();
    ui->scrapeButton->setText("Scraping...");
    moduleCourseMap = kmiljanScraper->download();
//    dodaj sve module u vektor da bi se indeksovalo u onom dropdownu intovima
    for (const auto& pair : moduleCourseMap) {
        modules.push_back(pair.first);
    }
    ui->scrapeButton->setText(oldText);
    ui->rasporedStartButton->setEnabled(true);
    ui->scheduleTable->setEnabled(true);
    ui->smerBox->setEnabled(true);
    ui->examArea->setEnabled(true);
}
