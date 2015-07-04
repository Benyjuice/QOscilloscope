#include "qoscscope.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QOscScope w;
    w.show();

    return a.exec();
}
