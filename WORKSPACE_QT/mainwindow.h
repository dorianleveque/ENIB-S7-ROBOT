#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define USE_LINUX 1

#include <QMainWindow>
#include <QKeyEvent>
#include <QPicture>
#include <iostream>
#include <QSlider>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>

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

    // Variables transmission
    QSerialPort *serial;
    QSerialPort *serialZigbee;
    QByteArray sendData;


    // FONCTIONS
    bool event(QEvent *event);
    void update();
    void setUp(bool set);
    void setDown(bool set);
    void setLeft(bool set);
    void setRight(bool set);
    void initProgram();
    void robotAvancer();
    void robotReculer();
    void robotGauche();
    void robotDroite();
    void robotStop();
    void openSerialPort();
    void onButSendClicked();
    void writeData(const QByteArray &data);
    void readData();
    void openZigbeePort();


};



#endif // MAINWINDOW_H
