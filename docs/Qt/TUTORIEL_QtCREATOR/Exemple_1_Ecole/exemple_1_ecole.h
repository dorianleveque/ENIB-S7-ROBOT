#ifndef EXEMPLE_1_ECOLE_H
#define EXEMPLE_1_ECOLE_H

#include <QMainWindow>

namespace Ui {
    class Exemple_1_Ecole;
}

class Exemple_1_Ecole : public QMainWindow
{
    Q_OBJECT

public:
    explicit Exemple_1_Ecole(QWidget *parent = 0);
    ~Exemple_1_Ecole();

public slots:
    void btn1Click();
    void timerRead();

private:
    Ui::Exemple_1_Ecole *ui;
};

#endif // EXEMPLE_1_ECOLE_H
