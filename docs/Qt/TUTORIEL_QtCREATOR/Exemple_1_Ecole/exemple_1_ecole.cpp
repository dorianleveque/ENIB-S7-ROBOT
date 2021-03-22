#include "exemple_1_ecole.h"
#include "ui_exemple_1_ecole.h"
#include <QTimer>

Exemple_1_Ecole::Exemple_1_Ecole(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Exemple_1_Ecole)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerRead()));
    timer->setInterval(1);
    timer->start();
}

Exemple_1_Ecole::~Exemple_1_Ecole()
{
    delete ui;
}

void Exemple_1_Ecole::btn1Click()
{
    ui->recepteur->setText(ui->emetteur->text());
    ui->emetteur->clear();
    ui->lcdNumero->display(ui->numero->value());
    ui->numero->setValue(0);
}
void Exemple_1_Ecole::timerRead()
{
    ui->lcdVertical->display(ui->verticalMobile->value());
}
