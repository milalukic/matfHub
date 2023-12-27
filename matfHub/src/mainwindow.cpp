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


    //connect(ui->listFileView, &QPushButton::clicked, this, &MainWindow::on_fileView_doubleClicked);

}

MainWindow::~MainWindow()
{
    //je l ovde ide "close without saving?"?
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












