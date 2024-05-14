#include "VTKGenerateAttribute.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VTKGenerateAttribute w;
    w.show();
    return a.exec();
}
