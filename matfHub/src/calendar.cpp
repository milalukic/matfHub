#include "../include/calendar.h"
#include "../ui_mainwindow.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

#include <QFile>

Calendar::Calendar(Ui::MainWindow* ui) {
    initializeMap();
    ui->calendarWidget->setSelectedDate(selectedDate);
    ui->textEdit_2->setPlainText(date_to_note.value(selectedDate));
}

void Calendar::dateChanged(Ui::MainWindow *ui, QDate date) {
    selectedDate = date;
    ui->textEdit_2->setPlainText(date_to_note.value(selectedDate));
}

void Calendar::textSaved(Ui::MainWindow *ui){
    date_to_note[selectedDate] = ui->textEdit_2->toPlainText();
}

// Cuvamo mapu u JSON formatu pri izlasku iz aplikacije
void Calendar::saveHistory(){
    QString fileName = "../matfHub/matfHub/map.txt";

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QTextStream out(&file);

    // Moram da konvertujem QDate u format pogodan za JSON
    QMap<QString, QVariant> jsonMap;
    for (auto it = date_to_note.begin(); it != date_to_note.end(); ++it) {
        QString dateString = it.key().toString("yyyy-MM-dd");
        jsonMap.insert(dateString, it.value());
    }

    QJsonObject jsonObject = QJsonObject::fromVariantMap(jsonMap);
    QJsonDocument jsonDoc(jsonObject);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    out << jsonString;

    file.close();
}

// Inicijalizuje mapu sa prethodnom istorijom
void Calendar::initializeMap(){
    QFile jsonFile("../matfHub/matfHub/map.txt");

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
        QString note = it.value().toString();
        date_to_note.insert(date, note);
    }
}
