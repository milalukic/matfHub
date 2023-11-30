#ifndef NOTES_H
#define NOTES_H

#include <QString>
#include <QFileDialog>

#include "../ui_mainwindow.h"


class Notes
{
public:
    Notes();
    QString currentFile = "";

    void open_file(QString filePath, Ui::MainWindow *ui, QWidget *parent);

    void new_clicked(Ui::MainWindow *ui);
    void open_clicked(Ui::MainWindow *ui, QWidget *parent);
    void save_clicked(Ui::MainWindow *ui, QWidget *parent);

    void copy_clicked(Ui::MainWindow *ui);
    void paste_clicked(Ui::MainWindow *ui);
    void cut_clicked(Ui::MainWindow *ui);

    void undo_clicked(Ui::MainWindow *ui);
    void redo_clicked(Ui::MainWindow *ui);    
};

#endif // NOTES_H
