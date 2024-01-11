#ifndef VIEWS_H
#define VIEWS_H

#include <QAbstractItemView>
#include <QDebug>
#include <QKeyEvent>
#include <QListView>
#include <QTableView>

class myListView : public QListView
{
  Q_OBJECT

public:
  myListView(QWidget *parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  int sizeList;
};

class myThumbnailView : public QListView
{
  Q_OBJECT

public:
  myThumbnailView(QWidget *parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  int sizeThumbnail;
  int spacing;
  int grid;
};

class myGridView : public QTableView
{
  Q_OBJECT

public:
  myGridView(QWidget *parent = nullptr);

protected:
  // void keyPressEvent(QKeyEvent *event) override;

private:
  // int sizeGrid;
};

#endif // VIEWS_H
