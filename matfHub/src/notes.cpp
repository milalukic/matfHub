#include "../include/notes.h"
#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../QSourceHighlite/qsourcehighliter.h"



Notes::Notes(Ui::MainWindow* mw)
    :m_mw(mw)
{

//    initLangsEnum();
//    initLangsComboBox();
//    initThemesComboBox();

    //set highlighter
//    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
//    ui->plainTextEdit->setFont(f);
    m_highliter = new QSourceHighlite::QSourceHighliter(m_mw->textEdit->document());

//    connect(ui->langComboBox,
//            static_cast<void (QComboBox::*) (const QString&)>(&QComboBox::currentTextChanged),
//            this, &MainWindow::languageChanged);
//    connect(ui->themeComboBox,
//            static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
//            this, &MainWindow::themeChanged);

//    ui->langComboBox->setCurrentText("Asm");
//    languageChanged("Asm");
    m_highliter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeCpp);
//    QFile f(QDir::currentPath() + "/../test_files/" + lang + ".txt");
//    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        const auto text = f.readAll();
//        ui->plainTextEdit->setPlainText(QString::fromUtf8(text));
//    }
//    f.close();

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

//void Notes::openClicked(Ui::MainWindow *ui, QWidget *parent){
//    QString fileName = QFileDialog::getOpenFileName(parent, "Otvori novu datoteku");
//    openFile(fileName, ui, parent);
//}

void Notes::changeLanguage(QString fileName){

    QString extension = fileName.right(fileName.length() - fileName.lastIndexOf("."));

    QStringList extensionList;
    extensionList << ".cpp" << ".hpp" << ".js" << ".c" << ".h" << ".php" << ".qml" << ".py" << ".rs" << ".jav" << ".cs" << ".go" << ".sql" << ".json" << ".xml" << ".html" << ".css" << ".ts" << ".yaml" << ".ini" << ".vex" << ".s" << ".lua";
    //                0         1         2        3       4       5         6         7        8        9         10       11       12        13         14        15         16        17       18         19        20        21      22;
    //1 ne znam kako drugacije da saltam string po switchu a da nije enum (sto je vrv ispravnije resenje)
    //2 svakako ova lista bi mozda pre trebalo da bude neka staticka konstantna lista klase umesto da se pravi u rantajmu ali to ostavljam nekome drugome (vrv sebi u buducnosti) da popravi B)
    // apdejt: lice samo napravi mapu string u int, ionako zelis da neke razlicite ekstenzije okidaju isti jezik
    //TODO: dodati ekstenzije za javu i ostale koje imaju istu sintaksu a fale ovde
    //TODO: dodati prepoznavanje simejka parsiranjem pocetka naziva, bash i ostalih skripti bez ekstenzije itd

    QSourceHighlite::QSourceHighliter::Language language;
    switch (extensionList.indexOf(extension)) {
    case 0:
        language = QSourceHighlite::QSourceHighliter::Language::CodeCpp;
        break;
    case 1:
        language = QSourceHighlite::QSourceHighliter::Language::CodeCpp;
        break;
    case 2:
        language = QSourceHighlite::QSourceHighliter::Language::CodeJs;
        break;
    case 3:
        language = QSourceHighlite::QSourceHighliter::Language::CodeC;
        break;
    case 4:
        language = QSourceHighlite::QSourceHighliter::Language::CodeC;
        break;
    case 5:
        language = QSourceHighlite::QSourceHighliter::Language::CodePHP;
        break;
    case 6:
        language = QSourceHighlite::QSourceHighliter::Language::CodeQML;
        break;
    case 7:
        language = QSourceHighlite::QSourceHighliter::Language::CodePython;
        break;
    case 8:
        language = QSourceHighlite::QSourceHighliter::Language::CodeRust;
        break;
    case 9:
        language = QSourceHighlite::QSourceHighliter::Language::CodeJava;
        break;
    case 10:
        language = QSourceHighlite::QSourceHighliter::Language::CodeCSharp;
        break;
    case 11:
        language = QSourceHighlite::QSourceHighliter::Language::CodeGo;
        break;
    case 12:
        language = QSourceHighlite::QSourceHighliter::Language::CodeSQL;
        break;
    case 13:
        language = QSourceHighlite::QSourceHighliter::Language::CodeJSON;
        break;
    case 14:
        language = QSourceHighlite::QSourceHighliter::Language::CodeXML;
        break;
    case 15:
        language = QSourceHighlite::QSourceHighliter::Language::CodeXML;
        break;
    case 16:
        language = QSourceHighlite::QSourceHighliter::Language::CodeCSS;
        break;
    case 17:
        language = QSourceHighlite::QSourceHighliter::Language::CodeTypeScript;
        break;
    case 18:
        language = QSourceHighlite::QSourceHighliter::Language::CodeYAML;
        break;
    case 19:
        language = QSourceHighlite::QSourceHighliter::Language::CodeINI;
        break;
    case 20:
        language = QSourceHighlite::QSourceHighliter::Language::CodeVex;
        break;
    case 21:
        language = QSourceHighlite::QSourceHighliter::Language::CodeAsm;
        break;
    case 22:
        language = QSourceHighlite::QSourceHighliter::Language::CodeLua;
        break;
    default:
        //TODO nisam nasao jezik koji ne boji nista tkd sad ako ne prepozna ekstenziju ostavlja prethodni
//        language = QSourceHighlite::QSourceHighliter::Language::Text
        return;
        break;
    }

    m_highliter->setCurrentLanguage(language);

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

        changeLanguage(fileName);

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

    changeLanguage(currentFile);
    //nesto mi se cini da ovo ne radi, ako je to tacno treba jelte popraviti
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
