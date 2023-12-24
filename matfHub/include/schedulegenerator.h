#ifndef GENERATOR_H
#define GENERATOR_H

#include "../include/course.h"
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
    std::vector<std::vector<int>> bitmap;

public:
    Generator(const std::vector<Course>& courseList);

    bool scheduleFilter(const std::vector<Course>& schedule);

    void find();

    void displaySchedule(QTableWidget* tableWidget);

private:
    void _find(size_t i);

    bool conflict(const Course& term);

    void place(const Course& term);

    void remove(const Course& term);
};

#endif // GENERATOR_H
