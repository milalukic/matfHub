#include "../include/calendar.h"
#include "../ui_mainwindow.h"

#include <fstream>

Calendar::Calendar(Ui::MainWindow* ui) {
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
