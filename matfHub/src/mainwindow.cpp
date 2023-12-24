#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/filemanager.hpp"
#include "../include/matrix.hpp"
#include "../include/parser.hpp"
#include "../include/plotter.hpp"
#include "../include/statistics.hpp"
#include "../include/calculator.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <stack>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <QDebug>
#include <string>
#include <matplot/matplot.h>

#define DEBUG (qDebug() << __FILE__ << ":" << __LINE__ << ":\t")

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
    connect(ui->pbChangeStat, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);

    connect(ui->pbParser, &QPushButton::clicked, this, &MainWindow::calculateRegular);

    connect(ui->pbMatrixTranspose, &QPushButton::clicked, this, &MainWindow::calculateMatrixTranspose);
    connect(ui->pbMatrixInverse, &QPushButton::clicked, this, &MainWindow::calculateMatrixInverse);
    connect(ui->pbMatrixDiag, &QPushButton::clicked, this, &MainWindow::calculateMatrixDiag);
    connect(ui->pbMatrixOne, &QPushButton::clicked, this, &MainWindow::calculateMatrixOne);

    connect(ui->pbMatrixLoad1, &QPushButton::clicked, this, &MainWindow::reshapeMatrix1);
    connect(ui->pbMatrixLoad2, &QPushButton::clicked, this, &MainWindow::reshapeMatrix2);

    connect(ui->pbMatrixAdd, &QPushButton::clicked, this, &MainWindow::calculateMatrixAdd);
    connect(ui->pbMatrixSubtract, &QPushButton::clicked, this, &MainWindow::calculateMatrixSubtract);
    connect(ui->pbMatrixMultiply, &QPushButton::clicked, this, &MainWindow::calculateMatrixMultiply);
    connect(ui->pbMatrixDivide, &QPushButton::clicked, this, &MainWindow::calculateMatrixDivide);

    connect(ui->pbSaveMatrix, &QPushButton::clicked, this, &MainWindow::saveMatrix);

    connect(ui->pbPlot, &QPushButton::clicked, this, &MainWindow::plot);
    connect(ui->pbSin, &QPushButton::clicked, this, &MainWindow::plotSin);
    connect(ui->pbCos, &QPushButton::clicked, this, &MainWindow::plotCos);
    connect(ui->pbTan, &QPushButton::clicked, this, &MainWindow::plotTan);
    connect(ui->pbLn, &QPushButton::clicked, this, &MainWindow::plotLn);
    connect(ui->pbLog, &QPushButton::clicked, this, &MainWindow::plotLog);
    connect(ui->pbExp, &QPushButton::clicked, this, &MainWindow::plotExp);
    connect(ui->pbAbs, &QPushButton::clicked, this, &MainWindow::plotAbs);
    connect(ui->pbNeg, &QPushButton::clicked, this, &MainWindow::plotNeg);
    connect(ui->pbSquare, &QPushButton::clicked, this, &MainWindow::plotSquare);
    connect(ui->pbRoot, &QPushButton::clicked, this, &MainWindow::plotRoot);
    connect(ui->pbPlotParse, &QPushButton::clicked, this, &MainWindow::plotParse);
    connect(ui->pbLinspace, &QPushButton::clicked, this, &MainWindow::plotLinspace);
    connect(ui->pbPlotSave, &QPushButton::clicked, this, &MainWindow::savePlotting);

    connect(ui->pbMean, &QPushButton::clicked, this, &MainWindow::statCalcMean);
    connect(ui->pbVariance, &QPushButton::clicked, this, &MainWindow::statCalcVariance);
    connect(ui->pbStd, &QPushButton::clicked, this, &MainWindow::statCalcStd);
    connect(ui->pbMedian, &QPushButton::clicked, this, &MainWindow::statCalcMedian);
    connect(ui->pbMode, &QPushButton::clicked, this, &MainWindow::statCalcMode);
    connect(ui->pbHist, &QPushButton::clicked, this, &MainWindow::statPlotHist);
    connect(ui->pbBar, &QPushButton::clicked, this, &MainWindow::statPlotBar);
    connect(ui->pbBox, &QPushButton::clicked, this, &MainWindow::statPlotBox);

    connect(ui->pbHistorySave, &QPushButton::clicked, this, &MainWindow::historySave);

}
//TODO move this, change serbian to english
// std::vector<double> cppSplit(std::string &s){

//     std::cout << s << std::endl;
//     //TODO more flexible?
//     std::string delimiter = ", ";

//     std::vector<double>niz;

//     size_t pos = 0;
//     std::string token;
//     while ((pos = s.find(delimiter)) != std::string::npos) {
//         token = s.substr(0, pos);
//         std::cout << token << " " << stod(token) << std::endl;
//         niz.push_back(stod(token));
//         s.erase(0, pos + delimiter.length());
//     }
//     niz.push_back(stod(s));

//     //TODO pointer?
//     return niz;
// }

//TODO study code
std::vector<double> cppSplit(const std::string &s) {
    std::cout << s << std::endl;

    // TODO: more flexible?
    std::string delimiter = ", ";

    std::vector<double> niz;

    std::istringstream ss(s);
    std::string token;

    // Temporarily set the locale to "C" to ensure '.' is used as the decimal point
    std::locale defaultLocale = std::locale::global(std::locale("C"));

    while (std::getline(ss, token, ',')) {
        // Trim whitespaces from the token
        token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);

        try {
            double value = std::stod(token);
            std::cout << token << " " << std::setprecision(15) << value << std::endl;
            niz.push_back(value);
        } catch (const std::invalid_argument& e) {
            // Handle invalid argument (e.g., if conversion fails)
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            // Handle out of range (e.g., if the value is too large or too small)
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
    }

    // Restore the original locale
    std::locale::global(defaultLocale);

    // TODO: pointer?
    return niz;
}

//TODO change name
std::vector<std::string> cppSplitString(std::string &s){

    //TODO more flexible?
    std::string delimiter = ", ";

    std::vector<std::string>niz;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        niz.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    niz.push_back(s);

    //TODO pointer?
    return niz;
}

void MainWindow::showMatrix(Matrix *m){

//    QString history = ui->tbHistory->toPlainText();

//    for(int i = 0; i < m->dimension1(); i++) {
//        for(int j = 0; j < m->dimension2(); j++) {
//            history += QString::number(m->data()(i,j));
//            history += " ";
//        }
//        history += '\n';
//    }
//    history += "------------------------\n";
//    ui->tbHistory->setText(history);
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

void MainWindow::on_saveToolbarButton_clicked()//TODDO save/save as? trenutno najlaksa opcija da se sacuva izmena jednog fajla u drugi je ctrl+a ctrl+c ctrl+n ctrl+v ctrl+s (takodje bilo bi kul da se prva tri dugmeta aktiviraju i na ctrl+n ctrl+s i ctrl+o
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
    else if(buttonText == "Statistics")
        ui->stackedWidget->setCurrentIndex(3);
}
//TODO global
Calculator *calculator = new Calculator();
History *history = History::getHistory();
Parser *parser = new Parser();
int linesWritten = 0;

void MainWindow::writeToHistoryTB(History* history) {
    auto vecHistory = history->history();
    for (int i = linesWritten; i < vecHistory.size(); i++)
        this->ui->tbHistory->append(QString::fromStdString(vecHistory[i]));
    linesWritten = vecHistory.size();
}
void MainWindow::calculateRegular(){
//    Parser *parser = new Parser();

    //TODO so it works with char*
    char *expr = ui->leParser->text().toStdString().data();
    double res = parser->eval_exp(expr);
    QString qres = QString::number(res);

    history->writeHistory(expr, qres.toStdString());
    writeToHistoryTB(history);

    ui->leParser->setText(qres);

//    std::cout << calculator->lastLine() << std::endl;

//    delete parser; parser = nullptr;
}

//TODO what to do with it
Matrix *m1;
Matrix *m2;

QString MainWindow::readM1Data(){
    QString res("");
    //ovaj kod ce verovatno biti uzasan i pun bagova, radna verzija
    auto rows = ui->scrollAreaM1widget->children().first();
    for(auto cols : rows->children()){
        for(auto field : cols->children()){
            QLineEdit* lEdit = qobject_cast<QLineEdit*>(field);
            res += lEdit->text();
        }
    }
    return res;
}
QString MainWindow::readM2Data(){
    QString res("");
    //ovaj kod ce verovatno biti uzasan i pun bagova, radna verzija
    auto rows = ui->scrollAreaM2widget->children().first();
    for(auto cols : rows->children()){
        for(auto field : cols->children()){
            QLineEdit* lEdit = qobject_cast<QLineEdit*>(field);
            res += lEdit->text();
        }
    }
    return res;
}

QStringList MainWindow::matrixStringToStringList(QString str){
    str.replace("\n", "");
    str.replace("|", "");
    QStringList strLst = str.split("\t");
    strLst.removeAll("");
    return strLst;
}

void MainWindow::reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos){

    auto [oldDim2, oldDim1] = (pos == 1 ? Matrix::getM1shape() : Matrix::getM2shape());
    if(dim1 == oldDim1 && dim2 == oldDim2){
        return;
    }

    unsigned realDim1 = (dim1 <= 25 ? dim1 : 25);
    unsigned realDim2 = (dim2 <= 25 ? dim2 : 25);

    pos == 1 ? Matrix::reshapeM1(realDim2, realDim1) : Matrix::reshapeM2(realDim2, realDim1);

    QVBoxLayout* rows;
    QWidget* scrollAreaWidget = (pos == 1 ? ui->scrollAreaM1widget : ui->scrollAreaM2widget);
    if(scrollAreaWidget->children().isEmpty()){
        rows = new QVBoxLayout;
        scrollAreaWidget->setLayout(rows);
    }else{
        rows = qobject_cast<QVBoxLayout*>(scrollAreaWidget->children().first());
        while(!rows->isEmpty()){
            auto row = rows->takeAt(0)->layout();
            while(!row->isEmpty()){
                delete row->takeAt(0)->widget();
            }
            delete row;
        }
    }
    QStringList content = matrixStringToStringList(pos == 1 ? (Matrix::m1toString()) : (Matrix::m2toString()));
    for(int i = 0; i < realDim1; ++i){
        QHBoxLayout* fields = new QHBoxLayout;
        for(int j = 0; j < realDim2; ++j){
            QLineEdit* field = new QLineEdit;
            field->setText(content.at(j+i*realDim2));
            if(pos == 1){
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    Matrix::setM1Data(field->text().toDouble(), i, j);
                    qDebug().noquote() << Matrix::m1toString();
                });
            }else{
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    Matrix::setM2Data(field->text().toDouble(), i, j);
                    qDebug().noquote() << Matrix::m2toString();
                });
            }

            field->setMaximumSize(64, 32);
            fields->addWidget(field);
        }
        rows->addLayout(fields);
    }
}

void MainWindow::reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos, QStringList content){//TODO napraviti enum za pos LEFT i RIGHT

    auto [oldDim2, oldDim1] = (pos == 1 ? Matrix::getM1shape() : Matrix::getM2shape());

    unsigned realDim1 = (dim1 <= 25 ? dim1 : 25);
    unsigned realDim2 = (dim2 <= 25 ? dim2 : 25);

    pos == 1 ? Matrix::reshapeM1(realDim2, realDim1) : Matrix::reshapeM2(realDim2, realDim1);
    DEBUG << content;

    QVBoxLayout* rows;
    QWidget* scrollAreaWidget = (pos == 1 ? ui->scrollAreaM1widget : ui->scrollAreaM2widget);
    if(scrollAreaWidget->children().isEmpty()){
        rows = new QVBoxLayout;
        scrollAreaWidget->setLayout(rows);
    }else{
        rows = qobject_cast<QVBoxLayout*>(scrollAreaWidget->children().first());
        while(!rows->isEmpty()){
            auto row = rows->takeAt(0)->layout();
            while(!row->isEmpty()){
                delete row->takeAt(0)->widget();
            }
            delete row;
        }
    }
    for(int i = 0; i < realDim1; ++i){
        QHBoxLayout* fields = new QHBoxLayout;
        for(int j = 0; j < realDim2; ++j){
            QLineEdit* field = new QLineEdit;
            field->setText(content.at(j+i*realDim2));
            if(pos == 1){
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    Matrix::setM1Data(field->text().toDouble(), i, j);
                    qDebug().noquote() << Matrix::m1toString();
                });
            }else{
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    Matrix::setM2Data(field->text().toDouble(), i, j);
                    qDebug().noquote() << Matrix::m2toString();
                });
            }

            field->setMaximumSize(64, 32);
            fields->addWidget(field);
        }
        rows->addLayout(fields);
    }
}

//void MainWindow::parseMatrix1(){
void MainWindow::reshapeMatrix1(){//preimenujte reshape -> resize ako vam ima vise smisla
    int dim1 = ui->leMatrixDim11->text().toInt();
    int dim2 = ui->leMatrixDim12->text().toInt();
    reshapeMatrix(dim1, dim2, 1);
}

void MainWindow::reshapeMatrix2(){//e ovo je bukvalno kopiran kod ajde molim vas da se to izdvoji u funkciju mene trenutno mrzi tako da u "vas" unutar "molim vas" uglavnom spadam ja ali slobodno ako se neko pojavi i ne daj Boze cita komentare
    int dim1 = ui->leMatrixDim21->text().toInt();
    int dim2 = ui->leMatrixDim22->text().toInt();
    reshapeMatrix(dim1, dim2, 2);
}

void MainWindow::calculateMatrixTranspose(){

//    m1 = m1->transpose();
//    std::cout << "Transposed matrix: " << std::endl;
//    std::cout << m1 << std::endl;
//    showMatrix(m1);
}
//TODO exceptions
void MainWindow::calculateMatrixInverse(){

//    m1 = m1->inverse();
//    std::cout << "Inverse" << std::endl;
//    std::cout << m1 << std::endl;
//    showMatrix(m1);
}

void MainWindow::calculateMatrixDiag(){

//    m1 = m1->diag();
//    std::cout << "Diag" << std::endl;
//    std::cout << m1 << std::endl;
//    showMatrix(m1);
}

void MainWindow::calculateMatrixOne(){

//    m1 = m1->ones();
//    std::cout << m1 << std::endl;
//    std::cout << "One" << std::endl;
//    showMatrix(m1);
}


//TODO save this somewhere?

void MainWindow::calculateMatrixAdd(){
    bool sameDim = Matrix::add();
    if(sameDim){
        // qDebug().noquote() << Matrix::m3toString();
        history->writeHistory("Sabiranje matrica:", Matrix::m3toString().toStdString());
        writeToHistoryTB(history);
    }else{
        qDebug() << "dimenzije matrica se ne poklapaju";
        //TODO -||-
    }
}

void MainWindow::calculateMatrixSubtract(){
    bool sameDim = Matrix::subtract();
    if(sameDim){
        qDebug().noquote() << Matrix::m3toString();
        history->writeHistory("Oduzimanje matrica:", Matrix::m3toString().toStdString());
        writeToHistoryTB(history);
    }else{
        qDebug() << "dimenzije matrica se ne poklapaju";
        //TODO -||-
    }
}

void MainWindow::calculateMatrixMultiply(){
    bool inverseDim = Matrix::multiply();
    if(inverseDim){
        qDebug().noquote() << Matrix::m3toString();
        history->writeHistory("Mnozenje matrica:", Matrix::m3toString().toStdString());
        writeToHistoryTB(history);
    }else{
        qDebug() << "dimenzije matrica se ne poklapaju";
        //TODO -||-
    }
}
//TODO exceptions
void MainWindow::calculateMatrixDivide(){

//    m1->data((*m1 / *m2)->data());
//    std::cout << *m1 / *m2 << std::endl;
//    std::cout << "Divide" << std::endl;
//    showMatrix(m1);
}
void MainWindow::saveMatrix(){
    unsigned index = Matrix::saveMatrix();
    QPushButton* loadLeft = new QPushButton("Ucitaj levo");
    QPushButton* loadRight = new QPushButton("Ucitaj desno");
    QHBoxLayout* savedM = new QHBoxLayout();
    auto layout = ui->savedMatricesLayout;
    layout->addLayout(savedM);
    QLabel* matrixName = new QLabel("#m");//TODO Mat::getSavedVectorLen.toStr
    savedM->addWidget(matrixName);
    savedM->addWidget(loadLeft);
    savedM->addWidget(loadRight);
    connect(loadLeft, &QPushButton::clicked, this, [this, index](){
        auto [d1, d2] = Matrix::loadLeft(index);
        loadMatrix(1, matrixStringToStringList(Matrix::getSaved(index)->toString()), d1, d2);
    });
    connect(loadRight, &QPushButton::clicked, this, [this, index](){
        auto [d1, d2] = Matrix::loadRight(index);
        loadMatrix(2, matrixStringToStringList(Matrix::getSaved(index)->toString()), d1, d2);
    });
}

void MainWindow::loadMatrix(unsigned int pos, QStringList strLst, unsigned d1, unsigned d2){
    reshapeMatrix(d1, d2, pos, strLst);
}

//////////////////////////////////////////////////////
//TODO global classes?

//plt = new Plotter("dd");
void MainWindow::plotLinspace(){
    //TODO GLOBAL CLASSES?
    plt = new Plotter("dd");

    double lowerBound = ui->leLinspaceLB->text().toDouble();
    double upperBound = ui->leLinspaceUB->text().toDouble();
    double numOfDots = ui->leLinspaceS->text().toDouble();
    plt->linSpace(lowerBound, upperBound, numOfDots);

    ui->leState->setText("x");
    ui->lbLin->setText("Vektor je uspesno ucitan");
    std::cerr << "Resetovan y" << std::endl;
    std::cerr <<"Postavljen linspace" << std::endl;
}

void MainWindow::plot(){
//    Plotter *plt = new Plotter("sin");

    (*plt)();
    std::cerr <<"Crtanje: " << std::endl;
    std::cerr << "\t" << ui->leLinspaceLB->text().toStdString() << " ";
    std::cerr << ui->leLinspaceUB->text().toStdString() << std::endl;

    std::string lowerBound = ui->leLinspaceLB->text().toStdString();
    std::string upperBound = ui->leLinspaceLB->text().toStdString();
    std::string numOfDots = ui->leLinspaceS->text().toStdString();
    std::string function = ui->leState->text().toStdString();
    std::string linspaceString = "Plot: (" + lowerBound + ", " + upperBound + ")";
    linspaceString += " Dots: " + numOfDots;

    history->writeHistory(linspaceString, function);

}


void MainWindow::plotSin(){

    ui->leState->setText("sin(" + ui->leState->text() + ")" );

    plt->transformData(sin);

}

void MainWindow::plotCos(){

    ui->leState->setText("cos(" + ui->leState->text() + ")" );

    plt->transformData(cos);
}

void MainWindow::plotTan(){

    ui->leState->setText("tan(" + ui->leState->text() + ")" );

    plt->transformData(tan);
}

void MainWindow::plotLn(){

    ui->leState->setText("ln(" + ui->leState->text() + ")" );

    plt->transformData(log);
}

void MainWindow::plotLog(){

    ui->leState->setText("log(" + ui->leState->text() + ")" );

    plt->transformData(log10);
}

void MainWindow::plotExp(){

    ui->leState->setText("e^(" + ui->leState->text() + ")" );

    plt->transformData(exp);
}

void MainWindow::plotAbs(){

    ui->leState->setText("|" + ui->leState->text() + "|" );

    plt->transformData(std::abs);
}

//TODO friendly function?
double square(double s){
    return pow(s, 2);
}
//TODO friendly function?

double negation(double s){
    return -s;
}

void MainWindow::plotNeg(){
    std::string state = ui->leState->text().toStdString();
    if (state[0] == '-') {
        std::string new_state = state.substr(2, state.length()-3);
        ui->leState->setText(QString::fromStdString(new_state));
    } else
        ui->leState->setText("-(" + ui->leState->text() + ")" );

    plt->transformData(negation);
}

void MainWindow::plotSquare(){

    ui->leState->setText("(" + ui->leState->text() + ")²" );

    plt->transformData(square);
}

void MainWindow::plotRoot(){

    ui->leState->setText("√(" + ui->leState->text() + ")" );

    plt->transformData(sqrt);
}

//TODO make this work
void MainWindow::plotParse(){


    //set x
    //x = set x
    //parse x

    double value = 5;
    char* expr1 = "x=5";
    char *expr = ui->leParser->text().toStdString().data();
    double x1;
    std::vector<double>xs = plt->xData();
    std::vector<double>ys = plt->xData();
    for(int i = 0; i<xs.size(); i++){
//        expr1[2] = xs[i];
        x1 = parser->eval_exp(expr1);
        ys[i] = parser->eval_exp(expr);
    }

    std::cout << parser->eval_exp(expr1) << std::endl;
    std::cout << parser->eval_exp(expr) << std::endl;
    std::cout << xs[0] << std::endl;
    std::cout << ys[0] << std::endl;
    plt->yData(ys);
    (*plt)();

//    plt->transformDataParser(parser->eval_exp(expr1), parser->eval_exp(expr));
}

void MainWindow::savePlotting(){
    plt->savePlot();
}

//stat
//TODO global?
Statistics *stat = new Statistics();

void MainWindow::statCalcMean(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->mean();

    //TODO ploymorph
    std::string hOutput = "Mean: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcVariance(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->variance();

    //TODO ploymorph
    std::string hOutput = "Variance: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcStd(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->std();

    //TODO ploymorph
    std::string hOutput = "Std: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcMedian(){

    //TODO razmisli
    // stat->loadData();
    
    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->median();

    //TODO ploymorph
    std::string hOutput = "Median: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcMode(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->mode();

    std::string hOutput = "Mode: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statPlotHist(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));
    
    stat->histogram();

    history->writeHistory("Histogram", input);
}

void MainWindow::statPlotBar(){

    auto input1 = ui->leStat->text().toStdString();
    auto input2 = ui->leStatNames->text().toStdString();
    stat->xData(cppSplit(input1));
    stat->textData(cppSplitString(input2));

    stat->barplot();

    history->writeHistory("Barplot", input1 + input2);
}

void MainWindow::statPlotBox(){
    auto input1 = ui->leStat->text().toStdString();
    auto input2 = ui->leStatNames->text().toStdString();
    stat->xData(cppSplit(input1));
    stat->textData(cppSplitString(input2));

    stat->boxplot();

    history->writeHistory("Boxplot", input1 + input2);
}

//HISTORY

void MainWindow::historySave(){
    history->saveHistory();
    std::cout << "History saved!" << std::endl;
}
