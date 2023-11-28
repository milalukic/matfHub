#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../ui_mainwindow.h"

class Schedule
{
public:
    Schedule();
    void changeSmer(Ui::MainWindow *ui, int index);
    void findSchedule(Ui::MainWindow *ui);
};

#endif // SCHEDULE_H
