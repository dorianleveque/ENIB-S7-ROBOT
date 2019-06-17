#include "mainwindow.h"
#include "ui_mainwindow.h"

//===================================================
#define ID_ROUES 'R'
#define ID_ROUES_UP 'A'
#define ID_ROUES_DOWN 'R'
#define ID_ROUES_LEFT 'G'
#define ID_ROUES_RIGHT 'D'
#define ID_ROUES_STOP 'S'

#define ID_TOURELLE 'T'
#define ID_TOURELLE_UP 'A'
#define ID_TOURELLE_DOWN 'R'
#define ID_TOURELLE_LEFT 'G'
#define ID_TOURELLE_RIGHT 'D'
#define ID_TOURELLE_STOP 'S'

#define ID_CONSIGNE 'C'
//===================================================

static QString nameport = "/dev/ttyACM0";
static QString zigbeePort = "/dev/ttyUSB0";


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->slider,SIGNAL(valueChanged(int)),this,SLOT(onSliderValueChanged(int)));
    initProgram();
    openSerialPort();
    openZigbeePort();
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
            sendRobotData(ID_ROUES,ID_ROUES_UP);
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            setDown(true);
            update();
            sendRobotData(ID_ROUES,ID_ROUES_DOWN);
            return true;
        }
        else if (ke->key() == Qt::Key_Left)
        {
            setLeft(true);
            update();
            sendRobotData(ID_ROUES,ID_ROUES_LEFT);
            return true;
        }
        else if (ke->key() == Qt::Key_Right)
        {
            setRight(true);
            update();
            sendRobotData(ID_ROUES,ID_ROUES_RIGHT);
            return true;
        }

        else if (ke->key() == Qt::Key_Z)
        {
            setTourelleUp(true);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_UP);
            return true;
        }
        else if (ke->key() == Qt::Key_S)
        {
            setTourelleDown(true);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_DOWN);
            return true;
        }
        else if (ke->key() == Qt::Key_Q)
        {
            setTourelleLeft(true);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_LEFT);
            return true;
        }
        else if (ke->key() == Qt::Key_D)
        {
            setTourelleRight(true);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_RIGHT);
            return true;
        }

        else
        {
            // Other key
            QByteArray dataZ = QByteArray("Z");
            serialZigbee->write(dataZ);
            qInfo()<<"Envoi au zigbee:"<<dataZ;
            return true;
        }
    }

    if (event->type() == QEvent::KeyRelease) // Le robot s'arrete
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);

        if (ke->key() == Qt::Key_Up)
        {
            setUp(false);
            update();
            sendRobotData(ID_ROUES,ID_ROUES_STOP);
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            setDown(false);
            update();
            sendRobotData(ID_ROUES,ID_ROUES_STOP);
            return true;
        }
        else if (ke->key() == Qt::Key_Left)
        {
            setLeft(false);
            update();
            sendRobotData(ID_ROUES,ID_ROUES_STOP);
            return true;
        }
        else if (ke->key() == Qt::Key_Right)
        {
            setRight(false);
            update();
            sendRobotData(ID_ROUES,ID_ROUES_STOP);
            return true;
        }

        else if (ke->key() == Qt::Key_Z)
        {
            setTourelleUp(false);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_STOP);
            return true;
        }
        else if (ke->key() == Qt::Key_S)
        {
            setTourelleDown(false);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_STOP);
            return true;
        }
        else if (ke->key() == Qt::Key_Q)
        {
            setTourelleLeft(false);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_STOP);
            return true;
        }
        else if (ke->key() == Qt::Key_D)
        {
            setTourelleRight(false);
            update();
            sendRobotData(ID_TOURELLE,ID_TOURELLE_STOP);
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
    if(isUp) // Mise à jour des variables clavier des roues
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

    if(isTourelleUp) // Mise à jour des vatriables clavier de la tourelle
    {
        ui->tourelleUp->setHidden(false);
        ui->tourelleDown->setHidden(true);
        ui->tourelleLeft->setHidden(true);
        ui->tourelleRight->setHidden(true);
    }
    else if(isTourelleDown)
    {
        ui->tourelleUp->setHidden(true);
        ui->tourelleDown->setHidden(false);
        ui->tourelleLeft->setHidden(true);
        ui->tourelleRight->setHidden(true);
    }
    else if(isTourelleLeft)
    {
        ui->tourelleUp->setHidden(true);
        ui->tourelleDown->setHidden(true);
        ui->tourelleLeft->setHidden(false);
        ui->tourelleRight->setHidden(true);
    }
    else if(isTourelleRight)
    {
        ui->tourelleUp->setHidden(true);
        ui->tourelleDown->setHidden(true);
        ui->tourelleLeft->setHidden(true);
        ui->tourelleRight->setHidden(false);
    }
    else
    {
        ui->tourelleUp->setHidden(true);
        ui->tourelleDown->setHidden(true);
        ui->tourelleLeft->setHidden(true);
        ui->tourelleRight->setHidden(true);
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

void MainWindow::setTourelleUp(bool set)
{
    if(set)
    {
        isTourelleUp=true;isTourelleDown=false;isTourelleLeft=false;isTourelleRight=false;
    }
    else
    {
        isTourelleUp=false;isTourelleDown=false;isTourelleLeft=false;isTourelleRight=false;
    }
}
void MainWindow::setTourelleDown(bool set)
{
    if(set)
    {
        isTourelleUp=false;isTourelleDown=true;isTourelleLeft=false;isTourelleRight=false;
    }
    else
    {
        isTourelleUp=false;isTourelleDown=false;isTourelleLeft=false;isTourelleRight=false;
    }
}
void MainWindow::setTourelleLeft(bool set)
{
    if(set)
    {
        isTourelleUp=false;isTourelleDown=false;isTourelleLeft=true;isTourelleRight=false;
    }
    else
    {
        isTourelleUp=false;isTourelleDown=false;isTourelleLeft=false;isTourelleRight=false;
    }
}
void MainWindow::setTourelleRight(bool set)
{
    if(set)
    {
        isTourelleUp=false;isTourelleDown=false;isTourelleLeft=false;isTourelleRight=true;
    }
    else
    {
        isTourelleUp=false;isTourelleDown=false;isTourelleLeft=false;isTourelleRight=false;
    }
}



void MainWindow::sendRobotData(char caractere1, char caractere2, char caractere3)
{
#if USE_LINUX
    sendData= QByteArray(&caractere1); // Si on envoie plusieurs caracteres, il faut le changer dans le programme carte. Sinon la callback ne se declenche pas.
    if(caractere2 != '\0')
    {
        sendData.append(&caractere2);
    }
    if(caractere3 != '\0')
    {
        sendData.append(&caractere3);
    }
    writeData(sendData);
    //readData(); // ça renvoie bien
#else
    qInfo()<<"Fonction avancer";
#endif
}

void MainWindow::writeData(const QByteArray &data) // Sur la serial normale
{
#if USE_LINUX
    serial->write(data);
    qInfo()<<"Message ecrit:"<<data;

#else
    qInfo()<<"Fonction write data";
#endif
}

void MainWindow::readData()
{
#if USE_LINUX
    QByteArray data = serial->readAll();
    qInfo()<<"Data receive:"<<data;
#else

#endif
}



void MainWindow::openSerialPort() // Il faut brancher une carte pour que cela fonctionne
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


void MainWindow::openZigbeePort() // Il faut brancher une carte pour que cela fonctionne
{
#if USE_LINUX
    serialZigbee = new QSerialPort(this);
    serialZigbee->setPortName(zigbeePort);
    serialZigbee->open(QIODevice::ReadWrite);

     if( serialZigbee->isOpen()==false)
     {
          serialZigbee->clearError();
          QMessageBox::critical(this, "Port error", "Port: "+zigbeePort);
          QMessageBox::information(this, "Port error", "Vérifier nom du port \n Fermer tout programme utilisant la lisaison RS232 "+zigbeePort);
      }
   else
     {
         QMessageBox::information(this, "Port open", " "+zigbeePort);
          serialZigbee->setBaudRate(QSerialPort::Baud9600);
          serialZigbee->setStopBits(QSerialPort::OneStop);
          serialZigbee->setParity(QSerialPort::NoParity);
          serialZigbee->setDataBits(QSerialPort::Data8);
          serialZigbee->setFlowControl(QSerialPort::NoFlowControl);
     }
#else
    qInfo()<<"Version windows de la fonction openZigbeePort. Nom du port: "<<zigbeePort;
#endif
}


void MainWindow::onSliderValueChanged(int value)
{
    qDebug()<<"Callback Slider";
    int32_t convertValue = (int32_t)value;
    int8_t poidsFort = (convertValue && 0x1111111100000000)>>8;
    int8_t poidsFaible = convertValue &&0x11111111;
    sendRobotData(ID_CONSIGNE,poidsFort,poidsFaible); // Pas besoin de les cast en char => même taille
}



