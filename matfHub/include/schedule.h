#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../ui_mainwindow.h"
#include "../include/course.h"
#include "kmiljanscraper.h"
#include <memory>
#include <unordered_map>

class Schedule
{
public:
    Schedule();
    void changeModule(Ui::MainWindow *ui, int index);
    void findSchedule(Ui::MainWindow *ui);
    void downloadSchedule(Ui::MainWindow *ui);
    void scrapeSchedule(Ui::MainWindow *ui);

private:
    std::unique_ptr<KmiljanScraper> kmiljanScraper;
    std::unordered_map<std::string, CourseSet> moduleCourseMap;
    std::vector<std::string> modules;
};

#endif // SCHEDULE_H
