#include "exemple_2_ecole.h"
#include <QVBoxLayout>			//Classe pour un arrangement vertical.
#include <QHBoxLayout>			//Classe pour un arrangement horizontal.

Exemple_2_Ecole::Exemple_2_Ecole(QWidget *parent)
    : QWidget(parent)
    , btn1(new QPushButton)				//Cr�ation d'un bouton.
    , emetteur(new QLineEdit)			//Cr�ation d'un champs de texte.
    , recepteur(new QLineEdit)			//Cr�ation d'un champs de texte.
    , numero(new QSpinBox)				//Cr�ation d'un compteur.
    , lcdNumero(new QLCDNumber)			//Cr�ation d'un afficheur LCD.
    , horizontalSlider(new QSlider)		//Cr�ation d'un mobile horizontal.
    , lcdSlider(new QLCDNumber)			//Cr�ation d'un afficheur LCD.
    , timer(new QTimer)					//Cr�ation d'une horloge.
{
    timer->start();			//D�marrage de l'horloge.
    createUI();				//Fonction pour cr�er la fen�tre du programme.
    connectUI();			//Fonction pour connecter les objets de la fen�tre.
}

Exemple_2_Ecole::~Exemple_2_Ecole()
{

}

void Exemple_2_Ecole::createUI()
{
    QHBoxLayout *layout1(new QHBoxLayout);					//Cr�ation d'une zone d'objets horizontale.
    layout1->addWidget(emetteur);							//Ajout du champs de texte.
    layout1->addWidget(btn1);								//Ajout du bouton.
    layout1->addWidget(recepteur);							//Ajout du champs de texte.

    QHBoxLayout *layout2(new QHBoxLayout);					//Cr�ation d'une zone d'objets horizontale.
    layout2->addWidget(numero);								//Ajout du compteur.
    layout2->addWidget(lcdNumero);							//AJout de l'afficheur LCD.

    QHBoxLayout *layout3(new QHBoxLayout);					//Cr�ation d'une zone d'objets horizontale.
    horizontalSlider->setOrientation(Qt::Horizontal);		//Orientation du mobile.
    layout3->addWidget(horizontalSlider);					//Ajout du mobile.
    layout3->addWidget(lcdSlider);							//Ajout de l'afficheur LCD.

    QVBoxLayout *mainLayout = new QVBoxLayout;				//Cr�ation d'une zone d'objets verticale.
    mainLayout->addLayout(layout1);							//Ajout d'une zone.
    mainLayout->addLayout(layout2);							//Ajout d'une zone.
    mainLayout->addLayout(layout3);							//Ajout d'une zone.
    setLayout(mainLayout);									//D�claration de la zone principale.
    setWindowTitle("Exemple_2_Ecole");						//Mise en place d'un titre pour la fen�tre.
}

void Exemple_2_Ecole::connectUI()
{
    connect(btn1, SIGNAL(clicked()), this, SLOT(btn1Click()));		//Connection du bouton avec le slot.
    connect(timer, SIGNAL(timeout()), this, SLOT(timerRead()));		//Connection de l'horloge avec le slot.
}

void Exemple_2_Ecole::btn1Click()
{
    recepteur->setText(emetteur->text());
    emetteur->clear();
    lcdNumero->display(numero->value());
    numero->setValue(0);
}

void Exemple_2_Ecole::timerRead()
{
    lcdSlider->display(horizontalSlider->value());
}
