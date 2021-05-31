#pragma comment (lib, "opengl32.lib")
#include "View.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    w.show();
    return a.exec();
}
