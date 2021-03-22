#include "exemple_2_ecole.h"
#include <QVBoxLayout>			//Classe pour un arrangement vertical.
#include <QHBoxLayout>			//Classe pour un arrangement horizontal.

Exemple_2_Ecole::Exemple_2_Ecole(QWidget *parent)
    : QWidget(parent)
    , btn1(new QPushButton)				//Création d'un bouton.
    , emetteur(new QLineEdit)			//Création d'un champs de texte.
    , recepteur(new QLineEdit)			//Création d'un champs de texte.
    , numero(new QSpinBox)				//Création d'un compteur.
    , lcdNumero(new QLCDNumber)			//Création d'un afficheur LCD.
    , horizontalSlider(new QSlider)		//Création d'un mobile horizontal.
    , lcdSlider(new QLCDNumber)			//Création d'un afficheur LCD.
    , timer(new QTimer)					//Création d'une horloge.
{
    timer->start();			//Démarrage de l'horloge.
    createUI();				//Fonction pour créer la fenêtre du programme.
    connectUI();			//Fonction pour connecter les objets de la fenêtre.
}

Exemple_2_Ecole::~Exemple_2_Ecole()
{

}

void Exemple_2_Ecole::createUI()
{
    QHBoxLayout *layout1(new QHBoxLayout);					//Création d'une zone d'objets horizontale.
    layout1->addWidget(emetteur);							//Ajout du champs de texte.
    layout1->addWidget(btn1);								//Ajout du bouton.
    layout1->addWidget(recepteur);							//Ajout du champs de texte.

    QHBoxLayout *layout2(new QHBoxLayout);					//Création d'une zone d'objets horizontale.
    layout2->addWidget(numero);								//Ajout du compteur.
    layout2->addWidget(lcdNumero);							//AJout de l'afficheur LCD.

    QHBoxLayout *layout3(new QHBoxLayout);					//Création d'une zone d'objets horizontale.
    horizontalSlider->setOrientation(Qt::Horizontal);		//Orientation du mobile.
    layout3->addWidget(horizontalSlider);					//Ajout du mobile.
    layout3->addWidget(lcdSlider);							//Ajout de l'afficheur LCD.

    QVBoxLayout *mainLayout = new QVBoxLayout;				//Création d'une zone d'objets verticale.
    mainLayout->addLayout(layout1);							//Ajout d'une zone.
    mainLayout->addLayout(layout2);							//Ajout d'une zone.
    mainLayout->addLayout(layout3);							//Ajout d'une zone.
    setLayout(mainLayout);									//Déclaration de la zone principale.
    setWindowTitle("Exemple_2_Ecole");						//Mise en place d'un titre pour la fenêtre.
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
