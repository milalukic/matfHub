#include "../include/views.h"

myListView::myListView(QWidget *parent) : QListView(parent) {
    sizeList = 60;
    this->setIconSize(QSize(60, 60));
}

void myListView::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Plus){
        this->sizeList += 5;
        this->setIconSize(QSize(this->sizeList, this->sizeList));
        qDebug() << "povecavamList" << sizeList;
    }else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus){
        if (this->sizeList > 60){
            this->sizeList -= 5;
            this->setIconSize(QSize(this->sizeList, this->sizeList));
        }
        qDebug() << "smanjujemList" << sizeList;
    }
    QListView::keyPressEvent(event);
}


myThumbnailView::myThumbnailView(QWidget *parent) : QListView(parent) {
    sizeThumbnail = 70;
}

void myThumbnailView::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Plus){
        this->sizeThumbnail += 5;
        this->setIconSize(QSize(this->sizeThumbnail, this->sizeThumbnail));
        qDebug() << "povecavamThumbnail" << sizeThumbnail;
    }else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus){
        if (this->sizeThumbnail > 60){
            this->sizeThumbnail -= 5;
            this->setIconSize(QSize(this->sizeThumbnail, this->sizeThumbnail));
        }
        qDebug() << "smanjujemThumbnail" << sizeThumbnail;
    }
    QListView::keyPressEvent(event);
}


myGridView::myGridView(QWidget *parent) : QTableView(parent) {
    sizeGrid = 30;
    this->setIconSize(QSize(30, 30));
}

void myGridView::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Plus){
        this->sizeGrid += 5;
        this->setIconSize(QSize(this->sizeGrid, this->sizeGrid));
        qDebug() << "povecavamGrid" << sizeGrid;
    }else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus){
        this->sizeGrid -= 5;
        this->setIconSize(QSize(this->sizeGrid, this->sizeGrid));
        qDebug() << "smanjujemGrid" << sizeGrid;
    }
    QTableView::keyPressEvent(event);
}
