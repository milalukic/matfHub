#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/filemanager.hpp"
#include "../include/matrix.hpp"
#include "../include/parser.hpp"
#include "../include/config.hpp"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
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

    QString hubPath = Config::getConfig()->getHubPath();

    // ako ne postoji dir MATF, pravimo ga
    if(!QDir(hubPath).exists()){//-||-
        qDebug() << hubPath;
        QDir().mkdir(hubPath);
    }

    notes = new class Notes();

    ui->dirView->setModel(m_fileManager->dirModel);
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);

    setUpFileView();



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
    ui->fileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u sledece prikazan folder
void MainWindow::on_forwardButton_clicked()
{
    m_fileManager->forwardButtonClicked();
    ui->fileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u glavni hab folder
void MainWindow::on_homeButton_clicked()
{
    m_fileManager->homeButtonClicked();
    ui->fileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u roditeljski folder
void MainWindow::on_dotDotButton_clicked()
{
    m_fileManager->dotDotButtonClicked();
    ui->fileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija ka unesenoj putanji, ako postoji folder na tom mestu
void MainWindow::on_currentFilePath_editingFinished()
{
    m_fileManager->currentFilePathEditingFinished();
    ui->fileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//kreiranje novog foldera unutar trenutnog
void MainWindow::on_newFolderButton_clicked()
{
    m_fileManager->createNewFolder();
}

//kreiranje popapa sa kontekst menijem u desnom pogledu
void MainWindow::on_fileView_customContextMenuRequested(const QPoint &pos)// TODO !!!!!! ovo bi bilo lepo takodje izmestiti u filemanager.cpp ali prvo connect je iz QObject ili tako necega treba to nasledjivanje srediti drugo poslednja linija funkcije (i jos neke) zove privatne metode privatnih polja iz ui pa bi i to trebalo prebroditi nekako nekim seterom
{//TODO pored komentara u prethodnoj liniji takodje ova f-ja postaje glomazna, bilo bi lepo mozda razdvojiti je u pozive f-ja koje pojedinacno prave akcije
    //TODO cut, copy, paste mozda?
    QMenu* menu = new QMenu(this);

    QAction* newFolderAction = new QAction("New Folder", this);
    menu->addAction(newFolderAction);
    connect(newFolderAction, &QAction::triggered, [=]() {
        m_fileManager->createNewFolder();
    });

    QAction* newDocumentAction = new QAction("New Document", this);
    menu->addAction(newDocumentAction);
    connect(newDocumentAction, &QAction::triggered, [=](){
        m_fileManager->createNewDocument();
    });

    QAction* selectAllAction = new QAction("Select All", this);
    menu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, [=](){
        ui->fileView->clearSelection();
        ui->fileView->selectAll();
    });

    QMenu* sortSubMenu = new QMenu("Sort by", menu);

    QAction* sortByName = new QAction("Name", this);
    sortSubMenu->addAction(sortByName);
    connect(sortByName, &QAction::triggered, [=](){
        m_fileManager->sortByName();
    });

    QAction* sortByDate = new QAction("Date", this);
    sortSubMenu->addAction(sortByDate);
    connect(sortByDate, &QAction::triggered, [=](){
        m_fileManager->sortByDate();
    });

    QAction* sortBySize = new QAction("Size", this);
    sortSubMenu->addAction(sortBySize);
    connect(sortBySize, &QAction::triggered, [=](){
        m_fileManager->sortBySize();
    });

    QAction* sortByType = new QAction("Type", this);
    sortSubMenu->addAction(sortByType);
    connect(sortByType, &QAction::triggered, [=](){
        m_fileManager->sortByType();
    });

    menu->addMenu(sortSubMenu);


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

//promena glavnog hab foldera
void MainWindow::on_actionChangeHubLocation_triggered()
{
    QString newHubPath = QFileDialog::getExistingDirectory(this, "Odaberi direktorijum");
    Config::getConfig()->setHubPath(newHubPath);
    m_fileManager->currPath = newHubPath;
    m_fileManager->hubPath = newHubPath;
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


int MainWindow::countSelected(const QTableView* view){

    return view->selectionModel()->selectedIndexes().length();

}

QModelIndex MainWindow::getSelectedIndex(const QTableView* view){
    return view->selectionModel()->selectedIndexes().first();
}

QModelIndexList MainWindow::getSelectedIndices(const QTableView* view){
    return view->selectionModel()->selectedIndexes();
}

void MainWindow::setUpFileView(/*tipPogleda*/){
    //if(tip == details){
    //QTableView* fileView = new itd itd
    //ui->fileViewLayout->children()->at(0) dilit
    //ui->fileViewLayout->addWidget(fileView);
    ui->fileView->setModel(m_fileManager->fileModel);
    ui->fileView->setRootIndex(m_fileManager->fileModel->setRootPath(m_fileManager->hubPath));
    ui->currentFilePath->setText(m_fileManager->currPath);
    ui->fileView->setSelectionMode(QAbstractItemView::ExtendedSelection);//klik odabere kliknutu i oddabere ostale; shift klik selektuje sve izmedju selektovane i kliknute, ctrl klik odabere kliknutu i ne oddabere ostale
    ui->fileView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->fileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);//TODO ovo ne radi za prvo pokretanje..
    ui->fileView->verticalHeader()->setVisible(false);

    connect(ui->fileView, &QTableView::doubleClicked, this, [this](){//TODO koji signal je odgovarajuc? nesto sto odgovara promeni m_currPath i nista drugo? komplikovan nacin bi bio currPath retvoriti u klasu koja pored stringa ima i signal koji se ovde salje pri promeni QStringa ali to me smara trenutno...
        ui->fileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    });
    connect(ui->fileView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](){
        qDebug() << "aaaaaa";
        QModelIndexList selectedList = ui->fileView->selectionModel()->selectedIndexes();//TODO kada se selektuje vise stvari nesto ozbiljno ne valja, nagadjam da je brz fiks da ne kazem ni ne sumnjam B)
        for(auto selected : selectedList){
            int row = selected.row();
            ui->fileView->selectRow(row);
        }
    });

    //else if(tip = list){ itd itd

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












