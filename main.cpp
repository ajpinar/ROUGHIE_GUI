#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("ROUGHIE V2 Control Panel");
    w.show();

    return a.exec();
}
