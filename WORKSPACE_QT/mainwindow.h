#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPicture>
#include <iostream>
#include <QSlider>

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
    // Variables robot
    float vitesseRobot;


    // FONCTIONS
    bool event(QEvent *event);
    void update();
    void setUp(bool set);
    void setDown(bool set);
    void setLeft(bool set);
    void setRight(bool set);
    void initProgram();

};



#endif // MAINWINDOW_H
