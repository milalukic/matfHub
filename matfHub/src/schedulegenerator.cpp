#include "../include/schedulegenerator.h"
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <QDebug>
#include <QTableWidget>

Generator::Generator(const std::vector<Course>& courseList) : courses(courseList) {
    // Initialize bitmap for scheduling
    bitmap.resize(5, std::vector<int>(13, 0)); // Assuming 5 days and 13 time slots
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

void Generator::find() {
    _find(0);
    std::sort(schedules.begin(), schedules.end(), [](const auto& lhs, const auto& rhs) {
        return std::accumulate(lhs.begin(), lhs.end(), 0,
                               [](int sum, const Course& course) { return sum + course.start; }) <
               std::accumulate(rhs.begin(), rhs.end(), 0,
                   [](int sum, const Course& course) { return sum + course.start; });
    });
}

void Generator::displaySchedule(QTableWidget* tableWidget) {
    if (!schedules.empty()) {
        const std::vector<Course>& schedule = schedules.front(); // Displaying the first schedule
        int row = 0;
        for (const auto& course : schedule) {
            int day = course.day;
            int start = course.start;
            int end = course.end;

            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(course.description));
            tableWidget->setItem(row, day, item); // Adjust columns based on day and timeslots
            for (int i = start; i < end; ++i) {
                QTableWidgetItem* subItem = new QTableWidgetItem("Busy");
                tableWidget->setItem(row, i, subItem); // Mark slots as busy
            }
            ++row;
        }
    }
}

void Generator::_find(size_t i) {
    if (i == courses.size()) {
        std::vector<Course> placedList;
        std::sort(placedList.begin(), placedList.end(), [](const auto& lhs, const auto& rhs) {
            return std::tie(lhs.day, lhs.end) > std::tie(rhs.day, rhs.end);
        });

        if (scheduleFilter(placedList)) {
            schedules.push_back(placedList);
        }
        return;
    }

//        for (const auto& term : courses[i].possibleTerms) {
//            if (schedules.size() < 200 && !conflict(term)) {
//                place(term);
//                _find(i + 1);
//                remove(term);
//            }
//        }
}

bool Generator::conflict(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        if (bitmap[term.day][i] == 1) {
            return true;
        }
    }
    return false;
}

void Generator::place(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        bitmap[term.day][i] = 1;
    }
    placed.insert(term.description);
}

void Generator::remove(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        bitmap[term.day][i] = 0;
    }
    placed.erase(term.description);
}
