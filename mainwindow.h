#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "modelreaction.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ModelReaction *model, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
    ModelReaction *model;
};
#endif // MAINWINDOW_H
