#include <ctime>
#include <QApplication>

#include "frame.hpp"

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    Frame *frame = new Frame();
    frame->showMaximized();
    return a.exec();
}
