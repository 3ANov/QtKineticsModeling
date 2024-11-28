#include "mainwindow.h"
#include "QMessageBox"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        ModelReaction model;
        MainWindow w(&model);
        w.show();
        return a.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Фатальная ошибка", e.what());
        return EXIT_FAILURE;
    }
}
