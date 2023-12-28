#include "../include/views.h"

#define viewSize 96

myListView::myListView(QWidget *parent) : QListView(parent) {
    sizeList = 60;
    this->setIconSize(QSize(60, 60));
    this->setSelectionRectVisible(true);
}

void myListView::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Plus){
        this->sizeList += 5;
        this->setIconSize(QSize(this->sizeList, this->sizeList));
        //qDebug() << "povecavamList" << sizeList;
    }else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus){
        if (this->sizeList > 60){
            this->sizeList -= 5;
            this->setIconSize(QSize(this->sizeList, this->sizeList));
        }
        //qDebug() << "smanjujemList" << sizeList;
    }
    QListView::keyPressEvent(event);
}


myThumbnailView::myThumbnailView(QWidget *parent) : QListView(parent) {
    sizeThumbnail = 70;
    this->setViewMode(QListView::IconMode);
    grid = 96;
    this->setGridSize(QSize(grid, grid));
    spacing = 15;
    this->setSpacing(spacing);
    this->setIconSize(QSize(sizeThumbnail, sizeThumbnail));
    this->setMovement(QListView::Snap);
    this->setResizeMode(QListView::Adjust);
    this->setTextElideMode(Qt::ElideMiddle);
    this->setWordWrap(false);
    this->setSelectionRectVisible(true);

}

void myThumbnailView::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Plus){
        this->sizeThumbnail += 5;
        this->setIconSize(QSize(this->sizeThumbnail, this->sizeThumbnail));
        grid += 5;
        this->setGridSize(QSize(grid, grid));
        spacing += 5;
        this->setSpacing(spacing);
        //qDebug() << "povecavamThumbnail" << sizeThumbnail;
    }else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus){
        if (this->sizeThumbnail > 60){
            this->sizeThumbnail -= 5;
            this->setIconSize(QSize(this->sizeThumbnail, this->sizeThumbnail));
            grid -= 5;
            this->setGridSize(QSize(grid, grid));
            spacing -= 5;
            this->setSpacing(spacing);
        }
        //qDebug() << "smanjujemThumbnail" << sizeThumbnail;
    }
    QListView::keyPressEvent(event);
}


myGridView::myGridView(QWidget *parent) : QTableView(parent) {
    //sizeGrid = 30;
    //this->setIconSize(QSize(30, 30));
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//ovo je ipak na kraju ispalo kao malo nepotrebna funkcionalnost

//void myGridView::keyPressEvent(QKeyEvent *event){
//    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Plus){
//        this->sizeGrid += 5;
//        this->setIconSize(QSize(this->sizeGrid, this->sizeGrid));
//        qDebug() << "povecavamGrid" << sizeGrid;
//    }else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus){
//        this->sizeGrid -= 5;
//        this->setIconSize(QSize(this->sizeGrid, this->sizeGrid));
//        qDebug() << "smanjujemGrid" << sizeGrid;
//    }
//    QTableView::keyPressEvent(event);
//}
