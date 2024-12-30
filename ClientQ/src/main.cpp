#include <QApplication>
#include "imsystem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IMSystem sys;

    return a.exec();
}
