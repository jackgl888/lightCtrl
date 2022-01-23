#include "pcdcfg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pcdCfg w;
    w.show();

    return a.exec();
}
