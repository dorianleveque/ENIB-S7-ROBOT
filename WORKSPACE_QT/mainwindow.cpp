#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent *event) // Fonction que l'on a
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);

        if (ke->key() == Qt::Key_Tab)
        {
            // special tab handling here
            return true;
        }

        else if (ke->key() == Qt::Key_Up)
        {
            setUp();
            printf("Up set !\n");
            //ui->keyboardUp->set; // Ca marche pas encore
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            setDown();
            return true;
        }
        else if (ke->key() == Qt::Key_Left)
        {
            setLeft();
            return true;
        }
        else if (ke->key() == Qt::Key_Right)
        {
            setRight();
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
        ui->keyboardUp->setLineWidth(3);
        ui->keyboardDown->setLineWidth(1);
        ui->keyboardLeft->setLineWidth(1);
        ui->keyboardRight->setLineWidth(1);
    }
    else if(isDown)
    {
        ui->keyboardUp->setLineWidth(1);
        ui->keyboardDown->setLineWidth(3);
        ui->keyboardLeft->setLineWidth(1);
        ui->keyboardRight->setLineWidth(1);
    }
    else if(isLeft)
    {
        ui->keyboardUp->setLineWidth(1);
        ui->keyboardDown->setLineWidth(1);
        ui->keyboardLeft->setLineWidth(3);
        ui->keyboardRight->setLineWidth(1);
    }
    else if(isRight)
    {
        ui->keyboardUp->setLineWidth(1);
        ui->keyboardDown->setLineWidth(1);
        ui->keyboardLeft->setLineWidth(1);
        ui->keyboardRight->setLineWidth(3);
    }
}

void MainWindow::setUp(){isUp=true;isDown=false;isLeft=false;isRight=false;}
void MainWindow::setDown(){isUp=false;isDown=true;isLeft=false;isRight=false;}
void MainWindow::setLeft(){isUp=false;isDown=false;isLeft=true;isRight=false;}
void MainWindow::setRight(){isUp=false;isDown=false;isLeft=false;isRight=true;}
