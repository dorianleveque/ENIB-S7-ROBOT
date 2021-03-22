#include <QtGui/QApplication>
#include "exemple_2_ecole.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Exemple_2_Ecole w;
    w.show();

    return a.exec();
}
