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
}

Course KmiljanScraper::tdToCourse(QString tdHtml, int day, int start){
    tdHtml = tdHtml.replace("<br>", "\n"); // lakse kad je new line jer nemamo child html elemente vise
    QRegularExpression colspanRegex("colspan=\"(\\d+)\"");
    QRegularExpressionMatch colMatch = colspanRegex.match(tdHtml);
    int length = (colMatch.hasMatch()) ? colMatch.captured(1).toInt() : 1;
    QRegularExpression tdRegex(">([^<]*)<"); // ovo radi sad jer smo radili replace
    QRegularExpressionMatch tdMatch = tdRegex.match(tdHtml);
    QString extractedContent;
    if (tdMatch.hasMatch()) {
        extractedContent = tdMatch.captured(1);
    } else {
        std::cerr << "Error parsing : " << tdHtml.toStdString() << std::endl;
        exit(-1);
    }
    QString lectureType = "predavanje";
    if(extractedContent.contains("(vežbe)")){
        extractedContent.replace("(vežbe)", "");
        lectureType = "vezbe";
    } else if(extractedContent.contains("(praktikum)")) {
        extractedContent.replace("(praktikum)", "");
        lectureType = "praktikum";
    }
    QStringList soup = extractedContent.split("\n");

    QString name = soup[0].simplified();
    QStringList qgroups = soup[1].split(",");
    std::set<std::string> groups;
    for(auto& gr: qgroups){
        groups.insert(gr.simplified().toStdString());
    }
    QStringList qsubgroups = soup[2].split(",");
    std::set<std::string> subgroups;
    for(auto& subgr : qsubgroups){
        subgroups.insert(subgr.simplified().toStdString());
    }
    QString staff = soup.length() > 3 ? soup[3].simplified() : "NULL";

    std::set<std::string> modules;
    std::set<int> years;

    std::map<std::string, std::string> moduleMap = {
        {"I", "Informatika"},
        {"R", "Racunarstvo"},
        {"M", "Teorijska"},
        {"N", "Primenjena"},
        {"V", "Aktuarska"},
        {"L", "Profesor"}
    };
    for(auto sg : qsubgroups){
        sg = sg.replace("O", "RMNVL");
        for(auto& kv : moduleMap){
            if(sg.contains(QString::fromStdString(kv.first))){
                modules.insert(kv.second);
            }
        }
    }
    for(auto& g : groups){
        int year = g[0]-'0';
        years.insert(year);
    }
    std::string placeholderClassroom = "add clsrm later";
    return Course(name.toStdString(),
                  day,
                  staff.toStdString(),
                  start,
                  length,
                  lectureType.toStdString(),
                  groups,
                  placeholderClassroom,
                  modules,
                  years,
                  subgroups);
}
QStringList splitByTd(QString text){
    QStringList lines;
    int startPos = 0;
    while ((startPos = text.indexOf("<td", startPos)) != -1) {
        int endPos = text.indexOf("<td", startPos + 1);
        if (endPos != -1) {
            lines << text.mid(startPos, endPos - startPos);
            startPos = endPos;
        } else {
            lines << text.mid(startPos); // Add the remaining part to the list
            break;
        }
    }
    return lines;
}

QString  convertCyrillicToLatin(const QString& input) {
    QMap<QString, QString> cyrillicToLatin = {
        {"а", "a"}, {"б", "b"}, {"в", "v"}, {"г", "g"}, {"д", "d"}, {"ђ", "đ"},
         {"е", "e"}, {"ж", "ž"}, {"з", "z"}, {"и", "i"}, {"ј", "j"}, {"к", "k"},
          {"л", "l"}, {"љ", "lj"}, {"м", "m"}, {"н", "n"}, {"њ", "nj"}, {"о", "o"},
           {"п", "p"}, {"р", "r"}, {"с", "s"}, {"т", "t"}, {"ћ", "ć"}, {"у", "u"},
            {"ф", "f"}, {"х", "h"}, {"ц", "c"}, {"ч", "č"}, {"џ", "dž"}, {"ш", "š"},
            {"А", "A"}, {"Б", "B"}, {"В", "V"}, {"Г", "G"}, {"Д", "D"}, {"Ђ", "Đ"},
             {"Е", "E"}, {"Ж", "Ž"}, {"З", "Z"}, {"И", "I"}, {"Ј", "J"}, {"К", "K"},
              {"Л", "L"}, {"Љ", "Lj"}, {"М", "M"}, {"Н", "N"}, {"Њ", "Nj"}, {"О", "O"},
               {"П", "P"}, {"Р", "R"}, {"С", "S"}, {"Т", "T"}, {"Ћ", "Ć"}, {"У", "U"},
                {"Ф", "F"}, {"Х", "H"}, {"Ц", "C"}, {"Ч", "Č"}, {"Џ", "Dž"}, {"Ш", "Š"},
                };


    QString result;
    for (int i = 0; i < input.length(); ++i) {
        QString currentChar = input.mid(i, 1);
        if (cyrillicToLatin.contains(currentChar)) {
            result += cyrillicToLatin[currentChar];
        } else {
            result += currentChar;
        }
    }
    return result;
}


CourseSet KmiljanScraper::tableToCourses(QString tableHtml, std::string classroom){
    CourseSet courses;
//    QStringList lines = tableHtml.split(QRegularExpression("[\r\n]"), Qt::SkipEmptyParts);
    tableHtml = tableHtml.replace(QRegularExpression("[\n\r]"), "");
    tableHtml = tableHtml.simplified();
    QStringList lines = splitByTd(tableHtml);
    QStringList days = {"Ponedeljak", "Utorak", "Sreda", "Četvrtak", "Petak"};
    int dayIdx = 0;
    int hour = 0;
    for(auto& line : lines) {
        if(line.contains("Превођење")){
            std::cout << 3;
        }
        if(line.contains("Prevođenje")){
            std::cout << 2;
        }
        std::cout << line.toStdString() << std::endl;
        if(line.startsWith("<td")){
            if(dayIdx < 5 && line.contains(">"+days[dayIdx]+"<")){
                dayIdx++;
                hour = 0;
            } else if(dayIdx == 0) {
                continue; // Trazimo ponedeljak...
            } else if(line.contains("&nbsp;")){
                hour++;
            } else {
                Course course = tdToCourse(line, dayIdx-1, hour);
                hour += course.duration;
                if(line.contains("SEMINAR")) {
                    continue; // preskacemo seminare al moramo ovde da bi znali duration
                }
                course.classroom = classroom;
                courses.insert(course);
            }
        }
    }
    return courses;
}

CourseSet KmiljanScraper::getAllCourses(std::unordered_map<std::string, std::string> roomMap) noexcept(false){
    CourseSet allCourses;
    for(const auto& room : roomMap){
        std::string roomName = room.first;
        std::string roomLink = room.second;
        QString htmlString = QString::fromStdString(requestHandler.getHtml(roomLink));
        QRegularExpression tableRegex(R"(<table border="1" bordercolor="#000000" cellspacing="0" cellpadding="0">(.*)</table>)",
                                 QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = tableRegex.match(htmlString);
        QString tableContent;
        if (match.hasMatch()) {
            tableContent = match.captured(1);
            qDebug() << "Extracted table content: " << roomLink;
//            qDebug().noquote() << tableContent;
//            exit(1);
        } else {
            qDebug() << "Table not found in HTML. " << roomLink;
            continue;
        }

        tableContent = convertCyrillicToLatin(tableContent);
        CourseSet roomCourses = tableToCourses(tableContent, roomName);
        allCourses.insert(roomCourses.begin(), roomCourses.end());

    }
    return allCourses;
}

std::unordered_map<std::string, CourseSet> KmiljanScraper::download() noexcept(false){
    std::string baseHtml = requestHandler.getHtml(baseLink);
    auto roomMap = getRooms(baseHtml);
    CourseSet allCourses = getAllCourses(roomMap);
    std::unordered_map<std::string, CourseSet> finalMap;
    for(auto& course : allCourses) {
        for(const auto& module : course.modules) {
            finalMap[module].insert(course);
        }
    }
    return finalMap;
}
