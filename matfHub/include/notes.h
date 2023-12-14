#ifndef NOTES_H
#define NOTES_H

#include <QString>
#include <QFileDialog>
#include <QSyntaxHighlighter>

#include "../ui_mainwindow.h"
#include "../QSourceHighlite/qsourcehighliter.h"

class Notes
{
private:

    Ui::MainWindow* m_mw;
    QSourceHighlite::QSourceHighliter* m_highliter;

    void changeLanguage(QString fileName);

public:
    Notes(Ui::MainWindow* mw);
    QString currentFile = "";

    void openFile(QString filePath, Ui::MainWindow *ui, QWidget *parent);

    void newClicked(Ui::MainWindow *ui);
    void openClicked(Ui::MainWindow *ui, QWidget *parent);
    void saveClicked(Ui::MainWindow *ui, QWidget *parent);

    void copyClicked(Ui::MainWindow *ui);
    void pasteClicked(Ui::MainWindow *ui);
    void cutClicked(Ui::MainWindow *ui);

    void undoClicked(Ui::MainWindow *ui);
    void redoClicked(Ui::MainWindow *ui);
};

#endif // NOTES_H
