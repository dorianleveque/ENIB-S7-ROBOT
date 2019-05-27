#include "mainwindow.h"
#include "ui_mainwindow.h"

static QString nameport = "/dev/ttyACM0";


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initProgram();
    openSerialPort();
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
    ui->labelTest->setHidden(false);
    //update();
/*
    QPicture pict; // A voir plus tard
    bool success;
    success = pict.load("C:/Users/Elouan/Documents/SEN S7/Projet Robot/ihm_projetsen/WORKSPACE_QT/Images/up.png");
    //printf("Reussite du chargement de l'image: %d\n",success);
    qDebug()<<"Reussite du chargement de l'image: "<<success;
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

void MainWindow::robotAvancer()
{

}
void MainWindow::robotReculer()
{

}
void MainWindow::robotGauche()
{

}
void MainWindow::robotDroite()
{

}

void MainWindow::robotStop()
{

}

// Les fonctions pour la tourelle vont etre à faire

void MainWindow::openSerialPort()
{
#if USE_LINUX
    serial = new QSerialPort(this);
    serial->setPortName(nameport);
    serial->open(QIODevice::ReadWrite);

     if( serial->isOpen()==false)
     {
          serial->clearError();
          QMessageBox::critical(this, "Port error", "Port: "+nameport);
          QMessageBox::information(this, "Port error", "Vérifier nom du port \n Fermer tout programme utilisant la lisaison RS232 "+nameport);
      }
   else
     {
         QMessageBox::information(this, "Port open", " "+nameport);
          serial->setBaudRate(QSerialPort::Baud115200);
          serial->setStopBits(QSerialPort::OneStop);
          serial->setParity(QSerialPort::NoParity);
          serial->setDataBits(QSerialPort::Data8);
          serial->setFlowControl(QSerialPort::NoFlowControl);
     }
#else
    qInfo()<<"Version windows de la fonction openSerialPort. Nom du port: "<<nameport;
#endif
}

void MainWindow::onButSendClicked()
{
    /*
    QString message=mes_to_send->text();
    writeData(message.toUtf8()); // QString --> QByteArray
    */
}


void MainWindow::writeData(const QByteArray &data)
{
    //serial->write(data);
}

void MainWindow::readData()
{
    //QByteArray data = serial->readAll();
    //mes_received->setText(data);
}
