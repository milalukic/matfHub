#include "../include/calendar.h"
#include "../ui_mainwindow.h"
#include "../include/config.hpp"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

#include <QFile>
#include <QDateEdit>
#include <QDate>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG (qDebug() << __FILENAME__ << ":" << __LINE__ << ":\t")

Calendar::Calendar(Ui::MainWindow* ui) {
    initializeMap(ui);
    ui->calendarWidget->setSelectedDate(selectedDate);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::DayOfWeek::Monday);

    ui->dateEdit->QDateEdit::setDate(selectedDate);

    for (auto itemStr : date_to_note[selectedDate]){
        QListWidgetItem* item = new QListWidgetItem(itemStr, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

void Calendar::dateChanged(Ui::MainWindow *ui, QDate date) {
    selectedDate = date;
    ui->dateEdit->QDateEdit::setDate(selectedDate);
    ui->calendarWidget->setSelectedDate(date);
    ui->listWidget->clear();

    for (auto itemStr : date_to_note[selectedDate]){
        QListWidgetItem* item = new QListWidgetItem(itemStr, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

void Calendar::taskAdded(Ui::MainWindow *ui){

    if(ui->dateEdit->date() != selectedDate){
        dateChanged(ui, ui->dateEdit->date());
    }

    QString itemString = ui->timeEdit->time().toString("HH:mm") + " " + ui->newItemEdit->toPlainText();
    date_to_note[selectedDate].append(itemString);
    date_to_note[selectedDate].sort();

    ui->listWidget->clear();

    for (auto itemStr : date_to_note[selectedDate]){
        QListWidgetItem* item = new QListWidgetItem(itemStr, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

    ui->newItemEdit->clear();
    ui->newItemEdit->setFocus();
}

void Calendar::removeTask(Ui::MainWindow *ui){

    if(ui->listWidget->currentItem()){
        QString itemString = ui->listWidget->currentItem()->text();
        date_to_note[selectedDate].removeOne(itemString);
        QListWidgetItem* item = ui->listWidget->takeItem(ui->listWidget->currentRow());
        delete item;
    }

}

void Calendar::removeAll(Ui::MainWindow *ui){
    ui->listWidget->clear();
    date_to_note[selectedDate].clear();
}

// Cuvamo mapu u JSON formatu pri izlasku iz aplikacije
void Calendar::saveHistory(){
    QString fileName = Config::getConfig()->getConfigPath() + "/map.txt";

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QTextStream out(&file);

    // Moram da konvertujem QDate u format pogodan za JSON
    QMap<QString, QVariant> jsonMap;
    for (auto it = date_to_note.begin(); it != date_to_note.end(); ++it) {
        QString dateString = it.key().toString("yyyy-MM-dd");
        jsonMap.insert(dateString, it.value().join("\n"));
    }

    QJsonObject jsonObject = QJsonObject::fromVariantMap(jsonMap);
    QJsonDocument jsonDoc(jsonObject);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    out << jsonString;

    file.close();
}

void Calendar::addCourse(QDate next_d, QString desc){
        if(date_to_note[next_d].empty()){
            QList<QString> note = {desc};
            date_to_note.insert(next_d, note);

        }else {
            date_to_note[next_d].append(desc);
        }

}

void Calendar::initializeMap(Ui::MainWindow *ui){
    QFile jsonFile(Config::getConfig()->getConfigPath() + "/map.txt");

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        QString dateString = it.key();
        QDate date = QDate::fromString(dateString, "yyyy-MM-dd"); // Parse string back to QDate
        QList<QString> note = it.value().toString().split("\n");
        date_to_note.insert(date, note);
    }


    QFile schedulePath(Config::getConfig()->getConfigPath() + "/raspored.json");

    if (!schedulePath.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray jsonDataSchedule = schedulePath.readAll();
    //DEBUG << jsonDataSchedule;
    schedulePath.close();

    QJsonDocument jsonDocSchedule = QJsonDocument::fromJson(jsonDataSchedule);
    if (jsonDocSchedule.isNull()) {
        return;
    }

    QJsonArray coursesArray = jsonDocSchedule.array();
    //DEBUG << coursesArray;

    int i = 0;
    for (const auto& courseData : coursesArray) {
        //DEBUG << i++;
        //DEBUG << "courseData:";
        //DEBUG << courseData;
        QJsonObject courseObject = courseData.toObject();
        //DEBUG << "courseObject";
        //DEBUG << courseObject;

        QString desc = courseObject["description"].toString();
        //DEBUG << "desc:";
        //DEBUG << desc;
        int d = courseObject["day"].toInt();

        QDate today = QDate::currentDate();
        QDate next_d = today.addDays(7+d-today.dayOfWeek());

        while(next_d < ui->calendarWidget->maximumDate()){
            addCourse(next_d, desc);
            next_d = next_d.addDays(7);
        }
    }
}
