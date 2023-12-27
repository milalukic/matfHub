#ifndef GENERATOR_H
#define GENERATOR_H

#include "course.h"
#include "helpers.h"
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <QDebug>
#include <QTableWidget>

class Generator {
private:
    std::vector<Course> courses;
    std::vector<std::vector<Course>> schedules;
    std::unordered_set<std::string> placed;
    std::vector<std::vector<bool>> bitmap;

public:
    Generator();
    bool scheduleFilter(const std::vector<Course>& schedule);
    void find(StrMap<StrMap<CourseSet>> courseTypeTermMap);
    void displaySchedule(QTableWidget* tableWidget);

private:
    void _find(StrMap<StrMap<CourseSet>>& courseTypeTermMap, std::vector<Course>& placedList, int i, int stop);
    bool conflict(const Course& term);
    void place(const Course& term);
    void remove(const Course& term);
};

#endif // GENERATOR_H
