#include "../include/notes.h"
#include "../ui_mainwindow.h"

#include <QFileDialog>


notes::notes() {
}

void notes::new_clicked(Ui::MainWindow *ui){
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void notes::open_clicked(Ui::MainWindow *ui, QWidget *parent){
    QString fileName = QFileDialog::getOpenFileName(parent, "Otvori novu datoteku");
    QFile file(fileName);
    currentFile = fileName;

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        return;
    }

    parent->setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();

    ui->textEdit->setText(text);
    file.close();
}

void notes::save_clicked(Ui::MainWindow *ui, QWidget *parent){
    QString fileName = QFileDialog::getSaveFileName(parent, "Sacuvaj.");
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        return;
    }

    currentFile = fileName;
    parent->setWindowTitle(fileName);

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void notes::copy_clicked(Ui::MainWindow *ui){
    ui->textEdit->copy();
}
void notes::paste_clicked(Ui::MainWindow *ui){
    ui->textEdit->paste();
}
void notes::cut_clicked(Ui::MainWindow *ui){
    ui->textEdit->cut();
}

void notes::undo_clicked(Ui::MainWindow *ui){
    ui->textEdit->undo();
}
void notes::redo_clicked(Ui::MainWindow *ui){
    ui->textEdit->redo();
}
