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
    std::string loadHtmlFromFile(const std::string& filePath);
    void saveHtmlToFile(const std::string& filePath, const std::string& content);
    std::string getHtmlContent();


private:
    std::string baseLink;
    RequestHandler requestHandler;
    std::unordered_map<std::string, std::string> getRooms(std::string baseHtml);
    Course tdToCourse(QString tdHtml, int day, int start);
    CourseSet tableToCourses(QString tableHtml, std::string classroom);
    CourseSet getAllCourses(std::unordered_map<std::string, std::string> roomMap) noexcept(false);
};

#endif // KMILJANSCRAPER_H
