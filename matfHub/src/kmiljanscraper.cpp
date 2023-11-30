#include "../include/kmiljanscraper.h"
#include "../include/course.h"
#include "../include/requesthandler.h"
#include "../include/exceptions.h"
#include <set>
#include <iostream>
#include <QRegularExpression>
#include <QXmlStreamReader>


KmiljanScraper::KmiljanScraper(){
    baseLink = "http://poincare.matf.bg.ac.rs/~miljan.knezevic/raspored/sve/";
};

std::unordered_map<std::string, std::string> KmiljanScraper::getRooms(std::string baseHtml){
    std::unordered_map<std::string, std::string> roomUrlMap;
    QRegularExpression regex("value=\"(room_\\d+\\.html)\">([^<]+)</OPTION");
    QRegularExpressionMatchIterator i = regex.globalMatch(QString::fromStdString(baseHtml));
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            QString roomLink = match.captured(1);
            QString value = match.captured(2);
            roomUrlMap[value.toStdString()] = baseLink + "/" + roomLink.toStdString();
        }
    }
    return roomUrlMap;
    for(const auto& kv : roomUrlMap){
        std::cerr << kv.first << "   " << kv.second << std::endl;
    }
}

CourseSet KmiljanScraper::traverseRooms(std::unordered_map<std::string, std::string> roomMap) noexcept(false){
    for(const auto& room : roomMap){
        std::string roomName = room.first;
        std::string roomLink = room.second;
        QString htmlString = QString::fromStdString(requestHandler.getHtml(roomLink));
//        QRegularExpression regex("<table border=\"1\" bordercolor=\"#000000\" cellspacing=\"0\" cellpadding=\"0\">(.*?)</table>",
//                                 QRegularExpression::DotMatchesEverythingOption);
//        QRegularExpressionMatch match = regex.match(htmlString);
//        QString tableContent;
//        if (match.hasMatch()) {
//            tableContent = match.captured(1);
//            qDebug() << "Extracted table content:";
//            qDebug().noquote() << tableContent;
//        } else {
//            qDebug() << "Table not found in HTML.";
//            exit(1);
//        }

//        QXmlStreamReader xml(tableContent);

//        QStringList courses;
//        while (!xml.atEnd() && !xml.hasError()) {
//            QXmlStreamReader::TokenType token = xml.readNext();

//            if (token == QXmlStreamReader::StartElement && xml.name() == QString("td")) {
//                QString course = xml.readElementText().trimmed();
//                if (!course.isEmpty() && course != QString("&nbsp;")) {
//                    courses.append(course);
//                }
//            }
//        }

//        if (xml.hasError()) {
//            qDebug() << "XML error:" << xml.errorString();
//        }

//        qDebug() << "Extracted Courses: ";
//        for (const QString &course : courses) {
//            qDebug() << course;
//        }

    }
    return {};
}


std::unordered_map<std::string, CourseSet> KmiljanScraper::download() noexcept(false){
    std::string baseHtml = requestHandler.getHtml(baseLink);
    auto roomMap = getRooms(baseHtml);
    CourseSet allCourses = traverseRooms(roomMap);
    std::unordered_map<std::string, CourseSet> finalMap;
    for(auto& course : allCourses) {
        for(const auto& module : course.modules) {
            finalMap[module].insert(course);
        }
    }
    return finalMap;
}
