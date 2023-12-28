#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../ui_mainwindow.h"
#include "helpers.h"
#include "course.h"
#include "schedulegenerator.h"
#include "kmiljanscraper.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>

class Schedule
{
public:
    Schedule();
    void changeModule(Ui::MainWindow *ui, int index);
    void findSchedule(Ui::MainWindow *ui);
    void downloadSchedule(Ui::MainWindow *ui);
    void scrapeSchedule(Ui::MainWindow *ui);
    void clearTable(Ui::MainWindow *ui);
    void nextSchedule(Ui::MainWindow *ui);
    void prevSchedule(Ui::MainWindow *ui);
    void saveSchedule(Ui::MainWindow *ui);
    void loadSchedule(Ui::MainWindow *ui);
    int brojRasporeda;
    Generator m_gen;

private:
    std::unique_ptr<KmiljanScraper> kmiljanScraper;
    StrMap<StrMap<StrMap<CourseSet>>> moduleCourseTypeTermMap;
    std::vector<std::string> modules;
    std::unordered_set<std::string> selectedCourses;
    std::string selectedModule;

//    State defaultTableState;
};

#endif // SCHEDULE_H
