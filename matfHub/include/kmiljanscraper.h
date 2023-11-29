#ifndef KMILJANSCRAPER_H
#define KMILJANSCRAPER_H

#include "course.h"
#include <set>
#include <string>

class KmiljanScraper{
public:
    KmiljanScraper();
    CourseSet downloadCourses();
    std::vector<std::string> downloadModules();

private:
    std::string baseLink;
};

#endif // KMILJANSCRAPER_H
