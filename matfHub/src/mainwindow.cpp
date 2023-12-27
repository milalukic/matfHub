#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/filemanager.hpp"
#include "../include/matrix.hpp"
#include "../include/parser.hpp"
#include "../include/config.hpp"

#include "../include/calendar.h"

#include "../include/plotter.hpp"
#include "../include/statistics.hpp"
#include "../include/calculator.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <stack>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <QDebug>
#include <string>
#include <matplot/matplot.h>

#include <fstream>
#include <map>

#define DEBUG (qDebug() << __FILE__ << ":" << __LINE__ << ":\t")

// #include <iostream>
// #include "../include/notes.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG (qDebug() << __FILENAME__ << ":" << __LINE__ << ":\t")

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

    // Kalendar
    calendar = new class Calendar(ui);

    // Notes
    notes = new class Notes();

    ui->dirView->setModel(m_fileManager->dirModel);
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);

    connect(ui->dirView, &QTreeView::doubleClicked, this, &MainWindow::dirViewDoubleClicked);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::backButtonClicked);
    connect(ui->forwardButton, &QPushButton::clicked, this, &MainWindow::forwardButtonClicked);
    connect(ui->homeButton, &QPushButton::clicked, this, &MainWindow::homeButtonClicked);
    connect(ui->dotDotButton, &QPushButton::clicked, this, &MainWindow::dotDotButtonClicked);
    connect(ui->newFolderButton, &QPushButton::clicked, this, &MainWindow::newFolderButtonClicked);

    connect(ui->currentFilePath, &QLineEdit::editingFinished, this, &MainWindow::currentFilePathEditingFinished);

    setUpFileView();




    //TODO FIX THIS!!!
//    connect(ui->pbMatrixTest, &QPushButton::clicked, this, &MainWindow::pbMatrixTest);
//TODO mozda neki connectCalc(this) koji ovo sve zove da bi konstruktor bio cistiji? slicno za ostale konekcije
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


    //connect(ui->listFileView, &QPushButton::clicked, this, &MainWindow::on_fileView_doubleClicked);

    //kalk

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
    calendar->saveHistory();
    delete ui;
}


// Funkcionalnosti Notes toolbara
void MainWindow::on_newFileNotesButton_clicked()
{
    notes->newClicked(ui);
}

void MainWindow::on_openFileNotesButton_clicked()
{
    notes->openClicked(ui, this);
}


void MainWindow::on_saveNotesButton_clicked()//save/save as? trenutno najlaksa opcija da se sacuva izmena jednog fajla u drugi je ctrl+a ctrl+c ctrl+n ctrl+v ctrl+s (takodje bilo bi kul da se prva tri dugmeta aktiviraju i na ctrl+n ctrl+s i ctrl+o
{
    notes->saveClicked(ui, this);
}

void MainWindow::on_copyNotesButton_clicked()
{
    notes->copyClicked(ui);
}

void MainWindow::on_pasteNotesButton_clicked()
{
    notes->pasteClicked(ui);
}

void MainWindow::on_cutNotesButton_clicked()
{
    notes->cutClicked(ui);
}

void MainWindow::on_undoNotesButton_clicked()
{
    notes->undoClicked(ui);
}

void MainWindow::on_redoNotesButton_clicked()
{
    notes->redoClicked(ui);
}


// funkc fMenadzera


//u levom pogledu dupli klik na folder smesta desni pogled u njega
void MainWindow::dirViewDoubleClicked(const QModelIndex &index)
{
    m_fileManager->dirViewDoubleClicked(index);
}

//u desnom pogledu dupli klik na stavku vrsi odgovarajucu akciju
void MainWindow::fileViewDoubleClicked(const QModelIndex &index)
{
    m_fileManager->fileViewDoubleClicked(index);
}

//navigacija u prethodno prikazani folder
void MainWindow::backButtonClicked()
{
    m_fileManager->backButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u sledece prikazan folder
void MainWindow::forwardButtonClicked()
{
    m_fileManager->forwardButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u glavni hab folder
void MainWindow::homeButtonClicked()
{
    m_fileManager->homeButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u roditeljski folder
void MainWindow::dotDotButtonClicked()
{
    m_fileManager->dotDotButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija ka unesenoj putanji, ako postoji folder na tom mestu
void MainWindow::currentFilePathEditingFinished()
{
    m_fileManager->currentFilePathEditingFinished();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//kreiranje novog foldera unutar trenutnog
void MainWindow::newFolderButtonClicked()
{
    m_fileManager->createNewFolder();
}

//kreiranje popapa sa kontekst menijem u desnom pogledu
void MainWindow::fileViewCustomContextMenuRequested(const QPoint &pos, QAbstractItemView* view)// TODO !!!!!! ovo bi bilo lepo takodje izmestiti u filemanager.cpp ali prvo connect je iz QObject ili tako necega treba to nasledjivanje srediti drugo poslednja linija funkcije (i jos neke) zove privatne metode privatnih polja iz ui pa bi i to trebalo prebroditi nekako nekim seterom
{//TODO pored komentara u prethodnoj liniji takodje ova f-ja postaje glomazna, bilo bi lepo mozda razdvojiti je u pozive f-ja koje pojedinacno prave akcije
    //TODO cut, copy, paste mozda?

    QMenu* menu = new QMenu(this);

    QAction* newFolderAction = new QAction("New Folder", this);
    menu->addAction(newFolderAction);
    connect(newFolderAction, &QAction::triggered, this, [=]() {
        m_fileManager->createNewFolder();
    });

    QAction* newDocumentAction = new QAction("New Document", this);
    menu->addAction(newDocumentAction);
    connect(newDocumentAction, &QAction::triggered, this, [=](){
        m_fileManager->createNewDocument();
    });

    QAction* selectAllAction = new QAction("Select All", this);
    menu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, this, [=](){
        for(int i = 0; i <  ui->fileViewLayout->count(); ++i){
            auto widget = ui->fileViewLayout->itemAt(i)->widget();
            QAbstractItemView* view = static_cast<QAbstractItemView*>(widget);
            view->clearSelection();
            view->selectAll();
        }
    });

    QMenu* sortSubMenu = new QMenu("Sort by", menu);

    QAction* sortByName = new QAction("Name", this);
    sortSubMenu->addAction(sortByName);
    connect(sortByName, &QAction::triggered, this, [=](){
        m_fileManager->sortByName();
    });

    QAction* sortByDate = new QAction("Date", this);
    sortSubMenu->addAction(sortByDate);
    connect(sortByDate, &QAction::triggered, this, [=](){
        m_fileManager->sortByDate();
    });

    QAction* sortBySize = new QAction("Size", this);
    sortSubMenu->addAction(sortBySize);
    connect(sortBySize, &QAction::triggered, this, [=](){
        m_fileManager->sortBySize();
    });

    QAction* sortByType = new QAction("Type", this);
    sortSubMenu->addAction(sortByType);
    connect(sortByType, &QAction::triggered, this, [=](){
        m_fileManager->sortByType();
    });

    menu->addMenu(sortSubMenu);

    QMenu* viewSubMenu = new QMenu("View", menu);

    QAction* list = new QAction("List", this);//ovo mozda ima smisla smestiti u petlju samo bi onda naziv akcije morao nekako da se dobije od parsiranja naziva ovih pogleda TOTO?
    viewSubMenu->addAction(list);
    connect(list, &QAction::triggered, this, [this](){MainWindow::showFileView(ui->listFileView);});
    QAction* table = new QAction("Details", this);
    viewSubMenu->addAction(table);
    connect(table, &QAction::triggered, this, [this](){MainWindow::showFileView(ui->detailsFileView);});
    QAction* thumbnails = new QAction("Thumbnails", this);
    viewSubMenu->addAction(thumbnails);
    connect(thumbnails, &QAction::triggered, this, [this](){MainWindow::showFileView(ui->thumbnailsFileView);});

    menu->addMenu(viewSubMenu);

    int noSelected = countSelected(view);
    if(noSelected == 1){
        QAction* renameAction = new QAction("Rename", this);
        menu->addAction(renameAction);
        connect(renameAction, &QAction::triggered, this, [=](){
            QModelIndex selectedIndex = getSelectedIndex(view);
            QString oldName = m_fileManager->getNameFromIndex(selectedIndex);
            QString newName = QInputDialog::getText(this, "Preimenuj fajl", "Nov Naziv:", QLineEdit::Normal, oldName);
            m_fileManager->renameSelectedFile(selectedIndex, newName);
        });
    }

    if(noSelected > 0){
        QAction* deleteAction = new QAction("Delete", this);
        menu->addAction(deleteAction);
        connect(deleteAction, &QAction::triggered, this, [=](){
            QModelIndexList selectedIndices = getSelectedIndices(view); //meni se indices vise svidja kao mnozina od index nego indexes ali Bojane slobodno promeni ako zelis nije mi bitno
            m_fileManager->deleteSelectedFiles(selectedIndices);
        });
    }

    menu->popup(view->viewport()->mapToGlobal(pos));

}

void MainWindow::showFileView(QAbstractItemView* view){

    for(int i = 0; i < ui->fileViewLayout->count(); ++i){
        QAbstractItemView* widget = static_cast<QAbstractItemView*>(ui->fileViewLayout->itemAt(i)->widget());
        widget->hide();
    }
    view->show();

}

//izlazak iz aplikacije, logicno
void MainWindow::actionExitTriggered()
{
    calendar->saveHistory();
    QApplication::quit();
}


void MainWindow::on_actionDark_Mode_triggered()
{
    QApplication::setStyle("Fusion");
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
void MainWindow::actionChangeHubLocationTriggered()
{
    QString newHubPath = QFileDialog::getExistingDirectory(this, "Odaberi direktorijum");
    Config::getConfig()->setHubPath(newHubPath);
    m_fileManager->currPath = newHubPath;
    m_fileManager->hubPath = newHubPath;
    ui->currentFilePath->setText(m_fileManager->currPath);
    ui->detailsFileView->setRootIndex(m_fileManager->fileModel->setRootPath(m_fileManager->currPath));
}



//geteri seteri i ostala narusavanja mejnovih privatnosti
void MainWindow::fileViewSetPath(const QString path){
    for(int i = 0; i < ui->fileViewLayout->count(); ++i){
        auto view = static_cast<QAbstractItemView*>(ui->fileViewLayout->itemAt(i)->widget());
        view->setRootIndex(m_fileManager->fileModel->setRootPath(path));
        if(QTableView* table = dynamic_cast<QTableView*>(view)){
            table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);//TODO iz nekog razloga cini mi se da ovo ne radi UVEK ako se prvi put ulazi u neki folder?
        }
    }
}
void MainWindow::currentFilePathSetPath(const QString path){
    ui->currentFilePath->setText(path);
}
QString MainWindow::currentFilePathGetPath(){
    return ui->currentFilePath->text();
}



int MainWindow::countSelected(const QAbstractItemView* view){
    return view->selectionModel()->selectedIndexes().length();
}

QModelIndex MainWindow::getSelectedIndex(const QAbstractItemView* view){
    return view->selectionModel()->selectedIndexes().first();
}

QModelIndexList MainWindow::getSelectedIndices(const QAbstractItemView* view){
    return view->selectionModel()->selectedIndexes();
}

void MainWindow::setUpFileView(/*tipPogleda*/){

    ui->currentFilePath->setText(m_fileManager->currPath);

    connect(ui->detailsFileView, &QTableView::doubleClicked, this, [this](){//TODO koji signal je odgovarajuc? nesto sto odgovara promeni m_currPath i nista drugo? komplikovan nacin bi bio currPath retvoriti u klasu koja pored stringa ima i signal koji se ovde salje pri promeni QStringa ali to me smara trenutno...
        ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    });

    for(int i = 0; i < ui->fileViewLayout->count(); ++i){
        auto widget = ui->fileViewLayout->itemAt(i)->widget();
        QAbstractItemView* view = static_cast<QAbstractItemView*>(widget);//auto ga vec kastuje u konkretan tip dakle QTableView, QListView itd B) ali kompajler u daljem kodu to ne zna nego je u fazonu ok ovo ce sigurno biti bar QObject tako da ipak mora da se kastuje sto je idiotski ali tako je D: jednom kada naprave funkcionalne procesore koji lenjo izracunavaju i kada ceo svet predje na normalne funkcionalne interpretirane jezike ovo nece biti problem ;)
        //qDebug() << view;//apdejt: i ovaj staticki kast ga kastuje u klasu koja treba da bude, nagadjam jer je AbstractView apstraktna klasa TODO nauci vise o cppu

        view->setModel(m_fileManager->fileModel);
        view->setRootIndex(m_fileManager->fileModel->setRootPath(m_fileManager->hubPath));
        view->setSelectionMode(QAbstractItemView::ExtendedSelection);

        //connect(ui->detailsFileView, &QTableView::doubleClicked, m_fileManager, &FileManager::fileViewDoubleClicked); ovo ne radi a zelim da proradi, TODO istraziti
        connect(view, &QAbstractItemView::doubleClicked, this, &MainWindow::fileViewDoubleClicked);
        connect(view, &QAbstractItemView::customContextMenuRequested, this, [this, view](QPoint pos){
            fileViewCustomContextMenuRequested(pos, view);
        });

        if(auto tableView = dynamic_cast<QTableView*>(view)){

            //tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
            //connect(tableView, &QTableView::doubleClicked, this, [tableView](){tableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);}); //TODO ovo ne radi dovoljno lepo
            tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
//            tableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);//TODO ovo ne radi za prvo pokretanje..

            tableView->verticalHeader()->setVisible(false);

            tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        }else if(auto listView = dynamic_cast<QListView*>(view)){

            if(listView == ui->thumbnailsFileView){
                auto thumbnail = static_cast<QListView*>(view);
                thumbnail->setViewMode(QListView::IconMode);
#define viewSize 96 //mnogo vece od ovoga (vec 128) smara jer New Document 1.txt staje ceo tekst, 96 mi je taman za dibagovanje ovog elide/wordWrap koji pokusavam da namestim. ako je tebi drugacije promeni slobodno
                //TODO bojane pls treba napraviti slot koji se aktivira na signal ctrl+'+' i ctrl+scrollUp koji povecava viewSize, analogno za '-'
                //takodje smisli neki lepsi naziv hshshs koji pocinje sa m_ i logicno premesti ga u lokalnu privatnu, hvala <3
                thumbnail->setGridSize(QSize(1.5*viewSize, viewSize));
                thumbnail->setIconSize(QSize(0.75*viewSize, 0.75*viewSize));
                thumbnail->setSpacing(16); //debljina tapacirunga izmedju polja grida, nepotrebno posto tapacirung simuliramo smanjenjem velicine ikone na 0.75 velicine polja ili koliko nam se vec svidi
                thumbnail->setMovement(QListView::Snap);
                thumbnail->setTextElideMode(Qt::ElideMiddle);//ElideNone
                thumbnail->setWordWrap(false); //true
                //poslednje dve linije ne rade kako treba... probaj sve 4 kombinacije elideNone elideMiddle wrapTrue wrapFalse i sta znam.. ja bih voleo da wordWrap true znaci da ispisuje u narednom redu ono sto ne staje ali nije tako... opcije su jos i ElideRight sto ima smisla mada ako imas vise fajlova sa slicnim pocetkom a razlikama na kraju sta znam npr bas za ovo New Folder 5 meni se vise svidja da vidim i kraj, pritom onda vidis ekstenzije nmp.. i cetvrta opcija ElideLeft koja je hahaa samo glupa
            }

            listView->setSelectionRectVisible(true);

        }//else if (auto blablaView = dynamic_cast<QBlaView*>(view)){}itd itd //sad razumem potrebu za nekim qt stylesheetom, pogledi u zavisnosti od toga kako se podese mogu biti istog tipa a potpuno drugacijih zeljenih ponasanja a budimo realni i ovaj dinamicki kas koliko god kul bio radi se tokom rantajma i spor je

    }

    //naredna linija zakomentarisana za lakse dibagovanje, TODO obrisati je kad tad
    //showFileView(ui->listFileView);

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
    double res = parser->evalExpression(expr);
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


// Kalendar
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    calendar->dateChanged(ui, date);
}

void MainWindow::on_pushButton_clicked()
{
    calendar->taskAdded(ui);
}

void MainWindow::plotSin(){

    ui->leState->setText("sin(" + ui->leState->text() + ")" );

    plt->transformData(sin);

}

void MainWindow::on_pushButton_2_clicked()
{
    calendar->removeTask(ui);
}

void MainWindow::on_pushButton_3_clicked()
{
    calendar->removeAll(ui);

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

    char *expr = strdup(ui->leState->text().toStdString().c_str());

    double tmp;
    std::vector<double> xs = plt->xData();
    std::vector<double> ys = plt->xData();

    std::cout << expr << std::endl;
    for (int i = 0; i < xs.size(); i++) {
        char tmp1[20]; // Adjust the size based on your needs
        strcpy(tmp1, "x=");
        strcat(tmp1, std::to_string(xs[i]).c_str());

        tmp = parser->evalExpression(tmp1);

        // expr remains unchanged
        std::cout << expr << std::endl;
        ys[i] = parser->evalExpression(expr);
    }

    // Don't forget to free the memory allocated for expr after its last usage
    free(expr);
//    std::cout << parser->evalExpression(expr) << std::endl;
    std::cout << xs[99] << std::endl;
    std::cout << ys[99] << std::endl;
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
