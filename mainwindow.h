#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // FONCTIONS

private:
    Ui::MainWindow *ui;

    // Variables clavier
    bool isUp, isDown, isLeft, isRight;

    // FONCTIONS
    bool event(QEvent *event);
    void update();
    void setUp();
    void setDown();
    void setLeft();
    void setRight();

};

#endif // MAINWINDOW_H
