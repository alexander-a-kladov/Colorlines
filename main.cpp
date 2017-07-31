#include <QApplication>
#include "color_wid.h"

int main(int argc, char **argv)
{
    QApplication app(argc,argv);
    ColorWid cw;
    cw.show();
    return app.exec();
}
