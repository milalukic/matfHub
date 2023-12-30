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
#include <QMessageBox>

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

void Generator::showPopupWithText(const QString& text) {
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setWindowTitle("Raspored");
    //    msgBox.setStyleSheet("QMessageBox { background-color: white; }");
    msgBox.adjustSize();
    msgBox.layoutDirection();
    msgBox.exec();
}

void Generator::find(StrMap<StrMap<CourseSet>> courseTypeTermMap) {
    int stop = 0;
    // koliko stvari treba da stavimo
    for(const auto& pair : courseTypeTermMap) {
        StrMap<CourseSet> types = pair.second;
        stop += types.size();
    }
    std::vector<Course> placedList;
    std::unordered_set<std::string> alreadySearched;
    _find(courseTypeTermMap, placedList, 0, stop, alreadySearched);
    std::sort(schedules.begin(), schedules.end(), [](const auto& lhs, const auto& rhs) {
        return std::accumulate(lhs.begin(), lhs.end(), 0,
                               [](int sum, const Course& course) { return sum + course.start; }) <
               std::accumulate(rhs.begin(), rhs.end(), 0,
                   [](int sum, const Course& course) { return sum + course.start; });
    });
}

QColor getPastelColor(QString str) {
    uint hash = qHash(str);
    int r = (hash & 0xFF) % 100 + 100;
    int g = ((hash >> 8) & 0xFF) % 100 + 100;
    int b = ((hash >> 16) & 0xFF) % 100 + 100;
    return QColor(r, g, b);
}

void Generator::displaySchedule(QTableWidget* tableWidget, int brojRasporeda) {
//    tableWidget->clearContents();
    if (!schedules.empty()) {
        std::vector<Course>& schedule = schedules[brojRasporeda];

        for (auto& course : schedule) {
            int day = course.day;
            int start = course.start;
            int end = course.end;
            QTableWidgetItem* subItem = new QTableWidgetItem(QString::fromStdString(course.description + '\n' + course.course_type +
                                                                                    '\n' + course.teacher +
                                                                                    '\n' + course.classroom));
            subItem->setBackground(QBrush(getPastelColor(QString::fromStdString(course.description))));
            subItem->setTextAlignment(Qt::AlignCenter);
            subItem->setFlags(subItem->flags() & ~Qt::ItemIsEditable);
            int columnSpan = end - start;
            tableWidget->setSpan(day, start, 1, columnSpan);
            tableWidget->setItem(day, start, subItem);
        }

        saved = schedule;
    } else {
        showPopupWithText("Raspored ne postoji!");
    }
}


void Generator::_find(StrMap<StrMap<CourseSet>>& courseTypeTermMap, std::vector<Course>& placedList, int i, int stop,
                      std::unordered_set<std::string> alreadySearched) {
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
            if (alreadySearched.find(course + type) != alreadySearched.end()) {
                continue;
            }
            for (const auto& term : terms) {
                bool notPlacedYet = placed.find(term.description + term.course_type) == placed.end();
                if (schedules.size() < 200 && notPlacedYet && !conflict(term)) {
                    place(term);
                    placedList.push_back(term);
                    if(placedList.size() == stop) {
                        schedules.push_back(placedList);
                    }
                    else {
                        _find(courseTypeTermMap, placedList, i + 1, stop, alreadySearched);
                    }
                    remove(term);
                    placedList.pop_back();
                }
            }
            alreadySearched.insert(course + type);
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
        courseObject["start"] = course.start + 8;
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
        showPopupWithText("Raspored sacuvan");
    } else {
        std::cerr << "Greska u ispisu" << std::endl;
    }


}

std::vector<Course> Generator::loadCoursesFromJson(const QString& filePath) {
    std::vector<Course> courses;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Greska pri otvaranju fajla" << std::endl;
        return courses;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull() || !document.isArray()) {
        std::cerr << "Los format" << std::endl;
        return courses;
    }

    QJsonArray coursesArray = document.array();

    for (const auto& courseData : coursesArray) {
        if (!courseData.isObject()) {
            std::cerr << "Losi podaci" << std::endl;
            continue;
        }

        QJsonObject courseObject = courseData.toObject();
        std::string desc = courseObject["description"].toString().toStdString();
        int d = courseObject["day"].toInt();
        std::string t = courseObject["teacher"].toString().toStdString();
        int s = courseObject["start"].toInt() - 8;
        int dur = courseObject["duration"].toInt();
        std::string type = courseObject["course_type"].toString().toStdString();
        std::set<std::string> grps;
        for (const auto& group : courseObject["groups"].toArray()) {
            grps.insert(group.toString().toStdString());
        }
        std::string clsrm = courseObject["classroom"].toString().toStdString();
        std::set<std::string> mods;
        for (const auto& mod : courseObject["modules"].toArray()) {
            mods.insert(mod.toString().toStdString());
        }
        std::set<int> yrs;
        for (const auto& year : courseObject["years"].toArray()) {
            yrs.insert(year.toInt());
        }
        std::set<std::string> subgrps;
        for (const auto& subgroup : courseObject["subgroups"].toArray()) {
            subgrps.insert(subgroup.toString().toStdString());
        }

        Course newCourse(desc, d, t, s, dur, type, grps, clsrm, mods, yrs, subgrps);
        courses.push_back(newCourse);
    }


    return courses;
}
