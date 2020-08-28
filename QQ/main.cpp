#include "personList.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    personList w;
    w.show();
    return a.exec();
}
