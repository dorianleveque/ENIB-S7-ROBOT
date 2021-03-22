#include <QtGui/QApplication>
#include "exemple_1_ecole.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Exemple_1_Ecole w;
    w.show();

    return a.exec();
}
