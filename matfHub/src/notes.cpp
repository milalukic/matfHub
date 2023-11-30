#include "../include/notes.h"
#include "../ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>


Notes::Notes() {
}

void Notes::newClicked(Ui::MainWindow *ui){
    if(QString::compare(ui->textEdit->toPlainText(), "")){
        //mozda neki would you like to save ako nije prazan tekst boks?
        currentFile.clear();
        ui->textEdit->setText(QString());
    }else{
    }
}

void Notes::openClicked(Ui::MainWindow *ui, QWidget *parent){
    QString fileName = QFileDialog::getOpenFileName(parent, "Otvori novu datoteku");
    if(QString::compare(fileName, "")){//bez provere je izbacivao gresku kada se korisnik predomisli i zatvori prozor za odabir datoteke
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
}

void Notes::saveClicked(Ui::MainWindow *ui, QWidget *parent){

    if (currentFile.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(parent, "Sacuvaj.");//mislim da treci argument ove f je moze primiti put direktorijuma u kome ce se otvoriti to cudo, treba napraviti geter za hubPath da se ovde prosledi
        QFile file(fileName);

        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            return;
        }

        currentFile = fileName;
        parent->setWindowTitle(fileName);

        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.close();
    } else {
        QFile file(currentFile);

        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            return;
        }

        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.close();
    }
}

void Notes::copyClicked(Ui::MainWindow *ui){
    ui->textEdit->copy();
}
void Notes::pasteClicked(Ui::MainWindow *ui){
    ui->textEdit->paste();
}
void Notes::cutClicked(Ui::MainWindow *ui){
    ui->textEdit->cut();
}

void Notes::undoClicked(Ui::MainWindow *ui){
    ui->textEdit->undo();
}
void Notes::redoClicked(Ui::MainWindow *ui){
    ui->textEdit->redo();
}
