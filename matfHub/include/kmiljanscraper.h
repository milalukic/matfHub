#ifndef KMILJANSCRAPER_H
#define KMILJANSCRAPER_H

#include "course.h"
#include "requesthandler.h"
#include <set>
#include <string>

class KmiljanScraper{
public:
    KmiljanScraper();
    std::unordered_map<std::string, CourseSet> download() noexcept(false);

private:
    std::string baseLink;
    RequestHandler requestHandler;
    std::unordered_map<std::string, std::string> getRooms(std::string baseHtml);
    CourseSet traverseRooms(std::unordered_map<std::string, std::string> roomMap) noexcept(false);
};

#endif // KMILJANSCRAPER_H
