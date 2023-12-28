#include "../include/schedulegenerator.h"
#include "../include/schedule.h"
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
#include <iostream>

#define ROWS 5
#define COLUMNS 13

Generator::Generator() {
    bitmap.resize(ROWS, std::vector<bool>(COLUMNS, false));
}

bool Generator::scheduleFilter(const std::vector<Course>& schedule) {
    size_t n = schedule.size();

    auto classroomGroup = [](const Course& course) -> int {
        if (course.classroom.empty()) return 0;
        if (course.classroom[0] == 'J') return 1;
        if (course.classroom[0] == 'N') return 2;
        return 0;
    };

    for (size_t i = 0; i < n - 1; ++i) {
        if (schedule[i].day == schedule[i + 1].day) {
            if (classroomGroup(schedule[i]) != classroomGroup(schedule[i + 1])) {
                if (schedule[i + 1].start - schedule[i].end < 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Generator::find(StrMap<StrMap<CourseSet>> courseTypeTermMap) {
    int stop = 0;
    // koliko stvari treba da stavimo
    for(const auto& pair : courseTypeTermMap) {
        StrMap<CourseSet> types = pair.second;
        stop += types.size();
    }
    std::vector<Course> placedList;
    _find(courseTypeTermMap, placedList, 0, stop);
    std::sort(schedules.begin(), schedules.end(), [](const auto& lhs, const auto& rhs) {
        return std::accumulate(lhs.begin(), lhs.end(), 0,
                               [](int sum, const Course& course) { return sum + course.start; }) <
               std::accumulate(rhs.begin(), rhs.end(), 0,
                   [](int sum, const Course& course) { return sum + course.start; });
    });
    std::cout << "Schedules found: " << schedules.size() << std::endl;
}

void Generator::displaySchedule(QTableWidget* tableWidget, int brojRasporeda) {

    std::cout << schedules.size() << " " << brojRasporeda << std::endl;

    tableWidget->clearContents();
    if (!schedules.empty()) {

        std::vector<Course>& schedule = schedules[brojRasporeda];

        for (auto& course : schedule) {
            int day = course.day;
            int start = course.start;
            int end = course.end;

            for (int i = start; i < end; ++i) {
                QTableWidgetItem* subItem = new QTableWidgetItem(QString::fromStdString(course.description + '\n' + course.course_type +
                                                                                        '\n' + course.classroom));
                tableWidget->setItem(day, i, subItem);
            }
        }

        saved = schedule;
    }
}

void Generator::_find(StrMap<StrMap<CourseSet>>& courseTypeTermMap, std::vector<Course>& placedList, int i, int stop) {
    if (i == stop) {
        std::cerr << "Shouldnt be reachable" << std::endl;
        return;
    }

    for (const auto& pair : courseTypeTermMap) {
        std::string course = pair.first;
        StrMap<CourseSet> typeTermMap = pair.second;
        for (const auto& pair : typeTermMap) {
            std::string type = pair.first;
            CourseSet terms = pair.second;
            for (const auto& term : terms) {
                bool notPlacedYet = placed.find(term.description + term.course_type) == placed.end();
                if (schedules.size() < 200 && notPlacedYet && !conflict(term)) {
                    place(term);
                    placedList.push_back(term);
                    if(placedList.size() == stop) {
                        schedules.push_back(placedList);
                        std::cout << placedList[0].start + 8 << " " << placedList[0].end + 8 << " " << placedList[0].day << std::endl;
                    }
                    else {
                        _find(courseTypeTermMap, placedList, i + 1, stop);
                    }
                    remove(term);
                    placedList.pop_back();
                }
            }
        }
    }
    return;
}

bool Generator::conflict(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        if (bitmap[term.day][i] == true) {
            return true;
        }
    }
    return false;
}

void Generator::place(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        bitmap[term.day][i] = true;
    }
    placed.insert(term.description + term.course_type);
}

void Generator::remove(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        bitmap[term.day][i] = false;
    }
    placed.erase(term.description + term.course_type);
}

void Generator::saveCoursesToJson(const QString& filePath) {

    QJsonArray coursesArray;

    for (auto& course : saved) {
        QJsonObject courseObject;
        courseObject["description"] = QString::fromStdString(course.description);
        courseObject["day"] = course.day;
        courseObject["teacher"] = QString::fromStdString(course.teacher);
        courseObject["start"] = course.start;
        courseObject["duration"] = course.duration;
        courseObject["end"] = course.end;
        courseObject["course_type"] = QString::fromStdString(course.course_type);

        QJsonArray groupsArray;
        for (const auto& grp : course.groups) {
            groupsArray.append(QString::fromStdString(grp));
        }
        courseObject["groups"] = groupsArray;

        courseObject["classroom"] = QString::fromStdString(course.classroom);

        QJsonArray modulesArray;
        for (const auto& mod : course.modules) {
            modulesArray.append(QString::fromStdString(mod));
        }
        courseObject["modules"] = modulesArray;

        QJsonArray yearsArray;
        for (const auto& yr : course.years) {
            yearsArray.append(yr);
        }
        courseObject["years"] = yearsArray;

        QJsonArray subgroupsArray;
        for (const auto& subgrp : course.subgroups) {
            subgroupsArray.append(QString::fromStdString(subgrp));
        }
        courseObject["subgroups"] = subgroupsArray;

        coursesArray.append(courseObject);
    }

    QJsonDocument doc(coursesArray);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << doc.toJson();
        file.close();
    } else {
        qDebug() << "Failed to open file for writing";
    }
}
