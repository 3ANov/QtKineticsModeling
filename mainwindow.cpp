#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "modelreaction.h"

#include <qstandarditemmodel.h>

MainWindow::MainWindow(ModelReaction *model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(model)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



