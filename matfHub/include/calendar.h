#ifndef CALENDAR_H
#define CALENDAR_H

#include <QString>
#include <QDateTime>

#include "../ui_mainwindow.h"

class Calendar
{
public:
    Calendar(Ui::MainWindow* mw);

    QMap<QDate, QString> date_to_note;
    QDate selectedDate = QDate::currentDate();

    void dateChanged(Ui::MainWindow *ui, QDate);
    void textSaved(Ui::MainWindow *ui);
    void saveHistory();
    void initializeMap();
};

#endif // CALENDAR_H
