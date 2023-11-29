#include "../include/kmiljanscraper.h"
#include "../include/course.h"
#include <set>
#include <iostream>

KmiljanScraper::KmiljanScraper(){
    baseLink = "http://poincare.matf.bg.ac.rs/~kmiljan/raspored/sve/";
};

CourseSet KmiljanScraper::downloadCourses(){
//    ye...
    return CourseSet{};
};

std::vector<std::string> KmiljanScraper::downloadModules(){
//    yep...
    return std::vector<std::string>{"Informatika", "Matematika"};
}
