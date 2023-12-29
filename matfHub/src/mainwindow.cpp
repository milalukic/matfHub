#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/filemanager.hpp"
#include "../include/matrix.hpp"
#include "../include/parser.hpp"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <stack>
#include <iostream>
#include <memory>
#include <QDebug>
#include <string>


// #include <iostream>
// #include "../include/notes.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    schedule = std::make_unique<Schedule>();

    m_fileManager = new FileManager(this);

    // ako ne postoji dir MATF, pravimo ga
    if(!QDir("MATF").exists()){
        QDir().mkdir("MATF");
    }

    notes = new class Notes();
    QString sPath = ""; //ovde kasnije dodati path i gurnuti ga u setRootPath

    //Ucitavanje rasporeda
    QString schedulePath = "../matfHub/raspored.json";
    QFile scheduleFile(schedulePath);
    if (scheduleFile.exists() && scheduleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        schedule->loadSchedule(ui);
        scheduleFile.close();
    }
    ui->dirView->setModel(m_fileManager->dirModel);
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);
    //ui->dirView->setRootIndex(dirModel->setRootPath(hubPath));
    ui->fileView->setModel(m_fileManager->fileModel);
    ui->fileView->setRootIndex(m_fileManager->fileModel->setRootPath(m_fileManager->hubPath));
    ui->currentFilePath->setText(m_fileManager->currPath);
    ui->fileView->setSelectionMode(QAbstractItemView::ExtendedSelection);//klik odabere kliknutu i oddabere ostale; shift klik selektuje sve izmedju selektovane i kliknute, ctrl klik odabere kliknutu i ne oddabere ostale

    //TODO FIX THIS!!!
//    connect(ui->pbMatrixTest, &QPushButton::clicked, this, &MainWindow::pbMatrixTest);

    connect(ui->pbChangeRegular, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangeMatrix, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangePlot, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);

    connect(ui->pbParser, &QPushButton::clicked, this, &MainWindow::calculateRegular);

    connect(ui->pbMatrixTranspose, &QPushButton::clicked, this, &MainWindow::calculateMatrixTranspose);
    connect(ui->pbMatrixInverse, &QPushButton::clicked, this, &MainWindow::calculateMatrixInverse);
    connect(ui->pbMatrixDiag, &QPushButton::clicked, this, &MainWindow::calculateMatrixDiag);
    connect(ui->pbMatrixOne, &QPushButton::clicked, this, &MainWindow::calculateMatrixOne);

    connect(ui->pbMatrixLoad1, &QPushButton::clicked, this, &MainWindow::parseMatrix1);
    connect(ui->pbMatrixLoad2, &QPushButton::clicked, this, &MainWindow::parseMatrix2);

    connect(ui->pbMatrixAdd, &QPushButton::clicked, this, &MainWindow::calculateMatrixAdd);
    connect(ui->pbMatrixSubtract, &QPushButton::clicked, this, &MainWindow::calculateMatrixSubtract);
    connect(ui->pbMatrixMultiply, &QPushButton::clicked, this, &MainWindow::calculateMatrixMultiply);
    connect(ui->pbMatrixDivide, &QPushButton::clicked, this, &MainWindow::calculateMatrixDivide);

    //raspored konektovi



}

MainWindow::~MainWindow()
{
    //je l ovde ide "close without saving?"?
    delete ui;
}

// Funkcionalnosti Notes toolbara
void MainWindow::on_newFileToolbarButton_clicked()
{
    notes->newClicked(ui);
}

void MainWindow::on_openFileToolbarButton_clicked()
{
    notes->openClicked(ui, this);
}

void MainWindow::on_saveToolbarButton_clicked()//save/save as? trenutno najlaksa opcija da se sacuva izmena jednog fajla u drugi je ctrl+a ctrl+c ctrl+n ctrl+v ctrl+s (takodje bilo bi kul da se prva tri dugmeta aktiviraju i na ctrl+n ctrl+s i ctrl+o
{
    notes->saveClicked(ui, this);
}

void MainWindow::on_copyToolbarButton_clicked()
{
    notes->copyClicked(ui);
}

void MainWindow::on_pasteToolbarButton_clicked()
{
    notes->pasteClicked(ui);
}

void MainWindow::on_cutToolbarButton_clicked()
{
    notes->cutClicked(ui);
}

void MainWindow::on_undoToolbarButton_clicked()
{
    notes->undoClicked(ui);
}

void MainWindow::on_redoToolbarButton_clicked()
{
    notes->redoClicked(ui);
}


// funkc fMenadzera
void MainWindow::on_dirView_clicked(const QModelIndex &index)
{
//ovo nam vr ne treba
}

//u levom pogledu dupli klik na folder smesta desni pogled u njega
void MainWindow::on_dirView_doubleClicked(const QModelIndex &index)
{
    m_fileManager->dirViewDoubleClicked(index);
}

//u desnom pogledu dupli klik na stavku vrsi odgovarajucu akciju
void MainWindow::on_fileView_doubleClicked(const QModelIndex &index)
{
    m_fileManager->fileViewDoubleClicked(index);
}

//navigacija u prethodno prikazani folder
void MainWindow::on_backButton_clicked()
{
    m_fileManager->backButtonClicked();
}

//navigacija u sledece prikazan folder
void MainWindow::on_forwardButton_clicked()
{
    m_fileManager->forwardButtonClicked();
}

//navigacija u glavni hab folder
void MainWindow::on_homeButton_clicked()
{
    m_fileManager->homeButtonClicked();
}

//navigacija u roditeljski folder
void MainWindow::on_dotDotButton_clicked()
{
    m_fileManager->dotDotButtonClicked();
}

//navigacija ka unesenoj putanji, ako postoji folder na tom mestu
void MainWindow::on_currentFilePath_editingFinished()
{
    m_fileManager->currentFilePathEditingFinished();
}

//kreiranje novog foldera unutar trenutnog
void MainWindow::on_newFolderButton_clicked()
{
    m_fileManager->createNewFolder();
}

//kreiranje popapa sa kontekst menijem u desnom pogledu
void MainWindow::on_fileView_customContextMenuRequested(const QPoint &pos)// !!!!!! ovo bi bilo lepo takodje izmestiti u filemanager.cpp ali prvo connect je iz QObject ili tako necega treba to nasledjivanje srediti drugo poslednja linija funkcije (i jos neke) zove privatne metode privatnih polja iz ui pa bi i to trebalo prebroditi nekako nekim seterom
{//pored komentara u prethodnoj liniji takodje ova f-ja postaje glomazna, bilo bi lepo mozda razdvojiti je u pozive f-ja koje pojedinacno prave akcije
    //TODO cut, copy, paste
    //TODO make new [...]
    //makar document (.txt) zbog notesa, jos nesto ako ima smisla u odnosu na to sta ostali naprave
    QMenu* menu = new QMenu(this);

    QAction* newFolderAction = new QAction("New Folder", this);
    menu->addAction(newFolderAction);
    connect(newFolderAction, &QAction::triggered, [=]() {//jednakost u prvim zagradama hvata sve promenjive iz pozivajuce funkcije u pozvanu, ne pitaj koja magija niti zasto je to potrebno..
        m_fileManager->createNewFolder();
    });

    QAction* selectAllAction = new QAction("Select All", this);
    menu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, [=](){
        ui->fileView->clearSelection();
        ui->fileView->selectAll();//ovo ne radi kako sam zamislio i blago je reci da mi ide na zivce :)
        //mozda i radi nego ne prikazuje sve kao plavo, to plavo mozda nije select kako ga mi shvatamo
        //to plavo tacno jeste selekt kako ga mi shvatamo, samo je trebalo podesiti seleksn mod pogledu. obrisi ova tri komentara kada procitas.
    });

    int noSelected = countSelected(ui->fileView);
    if(noSelected == 1){
        QAction* renameAction = new QAction("Rename", this);
        menu->addAction(renameAction);
        connect(renameAction, &QAction::triggered, [=](){
            QModelIndex selectedIndex = getSelectedIndex(ui->fileView);
            QString oldName = m_fileManager->getNameFromIndex(selectedIndex);
            QString newName = QInputDialog::getText(this, "Preimenuj fajl", "Nov Naziv:", QLineEdit::Normal, oldName);
            m_fileManager->renameSelectedFile(selectedIndex, newName);
        });
    }

    if(noSelected > 0){
        QAction* deleteAction = new QAction("Delete", this);
        menu->addAction(deleteAction);
        connect(deleteAction, &QAction::triggered, [=](){
            QModelIndexList selectedIndices = getSelectedIndices(ui->fileView); //meni se indices vise svidja kao mnozina od index nego indexes ali Bojane slobodno promeni ako zelis nije mi bitno
            m_fileManager->deleteSelectedFiles(selectedIndices);
        });
    }


    menu->popup(ui->fileView->viewport()->mapToGlobal(pos));


}


//naredne dve funkcije mozda treba pomeriti u filemanager klasu ali mislim da je to pametnije uraditi kada krenemo da spajamo grane u zavisnosti od toga sta odlucimo kako ce se ponasati meni bar glavnog prozora

//izlazak iz aplikacije, logicno
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


//raspored stvari


void MainWindow::on_smerBox_activated(int index)
{
    schedule->changeModule(ui, index);
}


void MainWindow::on_rasporedStartButton_clicked()
{
    schedule->findSchedule(ui);
}


void MainWindow::on_scrapeButton_clicked()
{
    schedule->scrapeSchedule(ui);
}

void MainWindow::on_sledeciButton_clicked()
{
    schedule->nextSchedule(ui);
}

void MainWindow::on_prethodniButton_clicked()
{
    schedule->prevSchedule(ui);
}

void MainWindow::on_SacuvajButton_clicked()
{
    schedule->saveSchedule(ui);
}

//promena glavnog hab foldera
void MainWindow::on_actionChangeHubLocation_triggered()
{
    QString newHubPath = QFileDialog::getExistingDirectory(this, "Odaberi direktorijum");
    m_fileManager->hubPath = newHubPath;   //hubPath treba uciniti trajnim nakon zatvaranja programa
    m_fileManager->currPath = m_fileManager->hubPath;
    ui->currentFilePath->setText(m_fileManager->currPath);
    ui->fileView->setRootIndex(m_fileManager->fileModel->setRootPath(m_fileManager->currPath));
}



//geteri seteri i ostala narusavanja mejnovih privatnosti
void MainWindow::fileViewSetPath(const QString path){
    ui->fileView->setRootIndex(m_fileManager->fileModel->setRootPath(path));
}
void MainWindow::currentFilePathSetPath(const QString path){
    ui->currentFilePath->setText(path);
}
QString MainWindow::currentFilePathGetPath(){
    return ui->currentFilePath->text();
}


int MainWindow::countSelected(const QListView* view){

    return view->selectionModel()->selectedIndexes().length();

}

QModelIndex MainWindow::getSelectedIndex(const QListView* view){
    return view->selectionModel()->selectedIndexes().first();
}

QModelIndexList MainWindow::getSelectedIndices(const QListView* view){
    return view->selectionModel()->selectedIndexes();
}


//kalkulator things
//void MainWindow::pbMatrixTest(){
//    Matrix *m1 = new Matrix(3, 3);
//    std::cout << m1 << std::endl;
//}


void MainWindow::changeStackedWidgetPage(){
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonText = buttonSender->text();
    std::cout << buttonText.toStdString() << std::endl;

    if(buttonText == "Regular calculator")
        ui->stackedWidget->setCurrentIndex(0);
    else if(buttonText == "Matrix calculator")
        ui->stackedWidget->setCurrentIndex(1);
    else if(buttonText == "Plotting calculator")
        ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::calculateRegular(){

    //TODO make x+5 work (if possible)
    Parser *parser = new Parser();

    //TODO so it works with char*
    char *expr = ui->leParser->text().toStdString().data();
    parser->eval_exp(expr);
    double res = parser->eval_exp(expr);
    QString qres = QString::number(res);

    QString history = ui->tbParser->toPlainText();

    history += expr;
    history += "\n";
    history += qres;
    history += "\n--------------\n";

    ui->leParser->setText(qres);
    ui->tbParser->setText(history);

    delete parser; parser = nullptr;
}

void MainWindow::calculateMatrixTranspose(){

    //TODO make this work (CMake probably)
    //    Matrix *m1 = new Matrix(5, 5);
    std::cout << "Transpose" << std::endl;
}

void MainWindow::calculateMatrixInverse(){
    std::cout << "Inverse" << std::endl;
}

void MainWindow::calculateMatrixDiag(){
    std::cout << "Diag" << std::endl;
}

void MainWindow::calculateMatrixOne(){
    std::cout << "One" << std::endl;
}

void MainWindow::parseMatrix1(){
    QString text = "Dim1: " + ui->leMatrixDim11->text() + " Dim2: " + ui->leMatrixDim12->text() + " data: " + ui->leMatrixData1->text();
    std::cout << text.toStdString() << std::endl;

}

void MainWindow::parseMatrix2(){
    QString text = "Dim1: " + ui->leMatrixDim21->text() + " Dim2: " + ui->leMatrixDim22->text() + " data: " + ui->leMatrixData2->text();
    std::cout << text.toStdString() << std::endl;
}

void MainWindow::calculateMatrixAdd(){
    std::cout << "Add" << std::endl;
}

void MainWindow::calculateMatrixSubtract(){
    std::cout << "Subtract" << std::endl;
}

void MainWindow::calculateMatrixMultiply(){
    std::cout << "Multiply" << std::endl;
}

void MainWindow::calculateMatrixDivide(){
    std::cout << "Divide" << std::endl;
}






