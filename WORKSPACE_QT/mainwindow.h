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

public slots:
    void onSliderValueChanged(int value);

private:
    Ui::MainWindow *ui;

    // Variables clavier
    bool isUp, isDown, isLeft, isRight;
    bool isTourelleUp, isTourelleDown, isTourelleLeft, isTourelleRight;
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
    void setTourelleUp(bool set);
    void setTourelleDown(bool set);
    void setTourelleLeft(bool set);
    void setTourelleRight(bool set);
    void initProgram();
    void sendRobotData(char caractere,char caractere2 = '\0', char caractere3 = '\0');
    void openSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void openZigbeePort();


};



#endif // MAINWINDOW_H
