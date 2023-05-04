#include "../include/guicontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUIController w;
    w.show();
    return a.exec();
}
