#include "../include/calendar.h"
#include "../ui_mainwindow.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

#include <QFile>
#include <QDateEdit>
#include <QDate>

Calendar::Calendar(Ui::MainWindow* ui) {
    initializeMap();
    ui->calendarWidget->setSelectedDate(selectedDate);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::DayOfWeek::Monday);

    ui->dateEdit->QDateEdit::setDate(selectedDate);

    for (auto item : date_to_note[selectedDate]){
        if(item != "\n")
            ui->listWidget->addItem(item);
    }
}

void Calendar::dateChanged(Ui::MainWindow *ui, QDate date) {
    selectedDate = date;
    ui->dateEdit->QDateEdit::setDate(selectedDate);
    ui->listWidget->clear();

    for (auto item : date_to_note[selectedDate]){
        if(item != "\n")
            ui->listWidget->addItem(item);
    }
}

void Calendar::taskAdded(Ui::MainWindow *ui){

    QString itemString = ui->timeEdit->time().toString("HH:mm") + " " + ui->textEdit_3->toPlainText();

    QListWidgetItem* item = new QListWidgetItem(itemString, ui->listWidget);


    date_to_note[selectedDate].append(itemString);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->textEdit_3->clear();
    ui->textEdit_3->setFocus();
}

void Calendar::removeTask(Ui::MainWindow *ui){
    QListWidgetItem* item = ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete item;
}

void Calendar::removeAll(Ui::MainWindow *ui){
    ui->listWidget->clear();
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
        jsonMap.insert(dateString, it.value().join("\n"));
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
        QList<QString> note = it.value().toString().split("\n");
        date_to_note.insert(date, note);
    }
}
