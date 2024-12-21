#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "modelreaction.h"

#include <QMainWindow>
#include <QTableWidgetItem>

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
    void on_countRowSpinBox_valueChanged(int arg1);
    void on_graphicsPushButton_clicked();
    void drawReactionGraphs(
        const QVector<double>& times,
        const QVector<double> inputConcentrationsA,
        const QVector<double>& concA,
        const QVector<double>& concB,
        const QVector<double>& concC,
        const QVector<double>& concD
    );

private:
    Ui::MainWindow *ui;
    ModelReaction *model;
};
#endif // MAINWINDOW_H
