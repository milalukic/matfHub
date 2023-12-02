#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void calculateRegular();
    void changeStackedWidgetPage();
    void calculateMatrixTranspose();
    void calculateMatrixInverse();
    void calculateMatrixDiag();
    void calculateMatrixOne();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
