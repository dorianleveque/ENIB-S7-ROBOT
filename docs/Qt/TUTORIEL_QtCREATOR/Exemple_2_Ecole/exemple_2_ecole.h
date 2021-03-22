#ifndef EXEMPLE_2_ECOLE_H
#define EXEMPLE_2_ECOLE_H

#include <QtGui/QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLCDNumber>
#include <QSlider>
#include <QTimer>

class Exemple_2_Ecole : public QWidget
{
    Q_OBJECT

public:
    Exemple_2_Ecole(QWidget *parent = 0);
    ~Exemple_2_Ecole();
    void createUI();
    void connectUI();

public slots:
    void btn1Click();
    void timerRead();

private:
    QPushButton* btn1;
    QLineEdit* emetteur;
    QLineEdit* recepteur;
    QSpinBox* numero;
    QLCDNumber* lcdNumero;
    QSlider* horizontalSlider;
    QLCDNumber* lcdSlider;
    QTimer* timer;
};

#endif // EXEMPLE_2_ECOLE_H
