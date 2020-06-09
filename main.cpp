#include "frame.h"
#include <ctime>

#include <QApplication>


int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    Frame *frame = new Frame();
    frame->show();
    return a.exec();
}
