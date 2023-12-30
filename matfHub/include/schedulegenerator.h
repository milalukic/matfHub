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
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

class Generator {
private:
    std::vector<Course> courses;
    std::unordered_set<std::string> placed;
    std::vector<std::vector<bool>> bitmap;

public:
    Generator();
    std::vector<std::vector<Course>> schedules;
    std::vector<Course> saved;
    void place(const Course& term);
    void remove(const Course& term);
    bool scheduleFilter(const std::vector<Course>& schedule);
    void find(StrMap<StrMap<CourseSet>> courseTypeTermMap);
    void displaySchedule(QTableWidget* tableWidget, int brojRasporeda);
    void saveCoursesToJson(const QString& filePath);
    void showPopupWithText(const QString& text);
    std::vector<Course> loadCoursesFromJson(const QString& filePath);


private:
    void _find(StrMap<StrMap<CourseSet>>& courseTypeTermMap, std::vector<Course>& placedList, int i, int stop,
               std::unordered_set<std::string> alreadySearched);
    bool conflict(const Course& term);
};

#endif // GENERATOR_H
