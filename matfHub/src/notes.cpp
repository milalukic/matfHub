#include "../include/notes.h"
#include "../ui_mainwindow.h"

Notes::Notes() {
}

void Notes::openFile(QString filePath, Ui::MainWindow *ui, QWidget *parent) {
    QFile file(filePath);


    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        return;
    }

    parent->setWindowTitle(filePath);
    QTextStream in(&file);
    QString text = in.readAll();

    ui->textEdit->setText(text);
    currentFile = filePath;
    file.close();
}


void Notes::newClicked(Ui::MainWindow *ui){
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void Notes::openClicked(Ui::MainWindow *ui, QWidget *parent){
    QString fileName = QFileDialog::getOpenFileName(parent, "Otvori novu datoteku");
    openFile(fileName, ui, parent);
}

void Notes::saveClicked(Ui::MainWindow *ui, QWidget *parent){

    if (currentFile.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(parent, "Sacuvaj.");
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
