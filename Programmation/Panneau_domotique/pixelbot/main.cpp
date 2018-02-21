#include "pixelbot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PixelBot w;
    w.show();

    return a.exec();
}
