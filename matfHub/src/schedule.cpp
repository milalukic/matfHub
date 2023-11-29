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
    if(index == 0) {
        return;
    }
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
//    ovo treba da skrejpuje sajt rasporeda na matfu
//    treba da se koristi neka biblioteka za slanje HTTP zahteva
//    pa jos neka za parsiranje HTML-a...
//    pa da se to sacuva i parsira kao struktura podataka kojoj moze da se pristupa na nivou predmeta...
    modules = kmiljanScraper->downloadModules();
    ui->smerBox->setPlaceholderText("Izaberi smer");
    ui->smerBox->clear();
    for(auto& module : modules){
        ui->smerBox->addItem(QString::fromStdString(module));
    }
//    ui->smerBox->addItems(modules);
//    return;
    CourseSet allCourses = kmiljanScraper->downloadCourses();

    for(auto& course : allCourses) {
        for(auto& module : modules) {
            if(course.modules.find(module) != course.modules.end()){
                moduleCourseMap[module].insert(course);
            }
        }
    }
};

void Schedule::scrapeSchedule(Ui::MainWindow *ui){
    QString oldText = ui->scrapeButton->text();
    ui->scrapeButton->setText("Scraping...");
    this->downloadSchedule(ui);
    ui->scrapeButton->setText(oldText);
    ui->rasporedStartButton->setEnabled(true);
    ui->scheduleTable->setEnabled(true);
    ui->smerBox->setEnabled(true);
    ui->examArea->setEnabled(true);
}
