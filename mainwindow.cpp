#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "modelreaction.h"
#include "exceptions.h"

#include <QMessageBox>
#include <qstandarditemmodel.h>

MainWindow::MainWindow(ModelReaction *model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(model)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    connect(ui->exitPushButton, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_countRowSpinBox_valueChanged(int arg1)
{
    int currentRowCount = ui->tableWidget->rowCount();

    if (arg1 > currentRowCount) {
        for (int i = currentRowCount; i < arg1; ++i) {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            for (int j = 0; j < ui->tableWidget->columnCount(); ++j) {
                QTableWidgetItem *item = new QTableWidgetItem();
                ui->tableWidget->setItem(i, j, item);
            }
        }
    } else if (arg1 < currentRowCount) {
        for (int i = currentRowCount; i > arg1; --i) {
            ui->tableWidget->removeRow(i - 1);
        }
    }
}

void MainWindow::on_graphicsPushButton_clicked()
{

    // double *
    // ui->tableWidget->ite
}


