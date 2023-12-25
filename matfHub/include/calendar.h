#ifndef CALENDAR_H
#define CALENDAR_H

#include <QString>
#include <QDateTime>

#include "../ui_mainwindow.h"

class Calendar
{
public:
    Calendar(Ui::MainWindow* mw);

    QMap<QDate, QList<QString>> date_to_note;
    QDate selectedDate = QDate::currentDate();

    void dateChanged(Ui::MainWindow *ui, QDate);
    void taskAdded(Ui::MainWindow *ui);
    void removeTask(Ui::MainWindow *ui);
    void removeAll(Ui::MainWindow *ui);
    void saveHistory();
    void initializeMap();
};

#endif // CALENDAR_H
