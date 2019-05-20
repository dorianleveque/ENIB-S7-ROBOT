#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initProgram();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initProgram()
{
    setUp(false);
    setDown(false);
    setRight(false);
    setLeft(false);
    update();
    /*
    QPicture pict; // A voir plus tard
    bool success;
    success = pict.load("/home/e2015/e5even/Documents/SEN S7/ihm_projetsen/WORKSPACE_QT/trollface.jpg");
    //printf("Reussite du chargement de l'image: %d\n",success);
    std::cout<<"Reussite du chargement de l'image: "<<success<<"\n";
    ui->labelTest->setPicture(pict);
    */
    ui->slider->setFocusPolicy(Qt::NoFocus); // Pour dire au slider de ne pas recuperer les evenements claviers pour lui tout seul
}

bool MainWindow::event(QEvent *event) // Fonction surchargée qui va gerer les evenemments clavier
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);

        if (ke->key() == Qt::Key_Up)
        {
            setUp(true);
            update();
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            setDown(true);
            update();
            return true;
        }
        else if (ke->key() == Qt::Key_Left)
        {
            setLeft(true);
            update();
            return true;
        }
        else if (ke->key() == Qt::Key_Right)
        {
            setRight(true);
            update();
            return true;
        }
        else
        {
            // Other key
            return true;
        }
    }

    if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);

        if (ke->key() == Qt::Key_Up)
        {
            setUp(false);
            update();
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            setDown(false);
            update();
            return true;
        }
        else if (ke->key() == Qt::Key_Left)
        {
            setLeft(false);
            update();
            return true;
        }
        else if (ke->key() == Qt::Key_Right)
        {
            setRight(false);
            update();
            return true;
        }
        else
        {
            // Other key
            return true;
        }
    }

    return QWidget::event(event);
}

void MainWindow::update()
{
    if(isUp)
    {
        ui->keyboardUp->setHidden(false);
        ui->keyboardDown->setHidden(true);
        ui->keyboardLeft->setHidden(true);
        ui->keyboardRight->setHidden(true);
    }
    else if(isDown)
    {
        ui->keyboardUp->setHidden(true);
        ui->keyboardDown->setHidden(false);
        ui->keyboardLeft->setHidden(true);
        ui->keyboardRight->setHidden(true);
    }
    else if(isLeft)
    {
        ui->keyboardUp->setHidden(true);
        ui->keyboardDown->setHidden(true);
        ui->keyboardLeft->setHidden(false);
        ui->keyboardRight->setHidden(true);
    }
    else if(isRight)
    {
        ui->keyboardUp->setHidden(true);
        ui->keyboardDown->setHidden(true);
        ui->keyboardLeft->setHidden(true);
        ui->keyboardRight->setHidden(false);
    }
    else // On est dans aucun des cas => toutes les touches relachées
    {
        ui->keyboardUp->setHidden(true);
        ui->keyboardDown->setHidden(true);
        ui->keyboardLeft->setHidden(true);
        ui->keyboardRight->setHidden(true);
    }
}


void MainWindow::setUp(bool set)
{
    if(set)
    {
        isUp=true;isDown=false;isLeft=false;isRight=false;
    }
    else
    {
        isUp=false;isDown=false;isLeft=false;isRight=false; // On met tout à false
    }
}
void MainWindow::setDown(bool set)
{
    if(set)
    {
        isUp=false;isDown=true;isLeft=false;isRight=false;
    }
    else
    {
        isUp=false;isDown=false;isLeft=false;isRight=false;
    }
}
void MainWindow::setLeft(bool set)
{
    if(set)
    {
        isUp=false;isDown=false;isLeft=true;isRight=false;
    }
    else
    {
        isUp=false;isDown=false;isLeft=false;isRight=false;
    }
}
void MainWindow::setRight(bool set)
{
    if(set)
    {
        isUp=false;isDown=false;isLeft=false;isRight=true;
    }
    else
    {
        isUp=false;isDown=false;isLeft=false;isRight=false;
    }
}

/*
bool QSlider::event(QEvent *event) override
{
    if ((event->type() == QEvent::KeyPress) || (event->type() == QEvent::KeyRelease))
    {
        return true;
    }
}
*/
