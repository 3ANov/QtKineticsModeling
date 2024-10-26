#include "mainwindow.h"
#include "QMessageBox"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ModelReaction model;
    MainWindow w(&model);
    w.show();
    return a.exec();
}
