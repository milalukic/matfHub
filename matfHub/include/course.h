#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <string>
#include <set>

class Course {
public:
    std::string description;
    int day;
    std::string teacher;
    int start;
    int duration;
    int end;
    std::string course_type;
    std::set<std::string> groups;
    std::string classroom;
    std::set<std::string> modules;
    std::set<int> years;
    std::set<std::string> subgroups;

    Course(
        const std::string& desc,
        int d,
        const std::string& t,
        int s,
        int dur,
        const std::string& type,
        const std::set<std::string>& grps,
        std::string& clsrm,
        const std::set<std::string>& mods,
        const std::set<int>& yrs,
        const std::set<std::string>& subgrps
        );
};

struct CourseComparator {
    bool operator()(const Course& lhs, const Course& rhs) const {
        return lhs.description < rhs.description;
    }
};

using CourseSet = std::set<Course, CourseComparator>;

#endif // COURSE_H
