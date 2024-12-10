#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "modelreaction.h"
#include "exceptions.h"
#include "reactionparametersdialog.h"

#include <QMessageBox>
#include <qstandarditemmodel.h>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QGraphicsScene>
#include <QValueAxis>


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

    const int concentrationColumn = 0;
    const int timeColumn = 1;

    int rowCount = ui->tableWidget->rowCount();

    if (rowCount < 2) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно данных для анализа.");
        return;
    }

    QVector<double> times;
    QVector<double> concentrations;

    try {
        for (int i = 0; i < rowCount; ++i) {
            QTableWidgetItem *timeItem = ui->tableWidget->item(i, timeColumn);
            QTableWidgetItem *concentrationItem = ui->tableWidget->item(i, concentrationColumn);

            if (!timeItem || !concentrationItem) {
                throw ValidationError("Пустые ячейки в строке " + std::to_string(i + 1) + ".", 1001);
            }

            bool timeOk, concOk;
            double timeValue = timeItem->text().toDouble(&timeOk);
            double concValue = concentrationItem->text().toDouble(&concOk);


            if (!timeOk || !concOk) {
                throw ValidationError("Некорректные данные в строке " + std::to_string(i + 1) + ".", 1002);
            }

            if (timeValue < 0) {
                throw ValidationError("Отрицательное время в строке " + std::to_string(i + 1) + ".", 1003);
            }
            if (concValue < 0) {
                throw ValidationError("Отрицательная концентрация в строке " + std::to_string(i + 1) + ".", 1004);
            }

            if (i > 0) {
                double prevTimeValue = times[i - 1];
                if (timeValue <= prevTimeValue) {
                    throw ValidationError("Нарушение последовательности времени в строке " + std::to_string(i + 1) + ".", 1005);
                }

                double prevConcValue = concentrations[i - 1];
                if (concValue == prevConcValue) {
                    throw ValidationError("Концентрации в соседних точках должны быть различны (строка " + std::to_string(i) + " и " + std::to_string(i + 1) + ").", 1006);
                }
            }

            times.append(timeValue);
            concentrations.append(concValue);
        }

        bool aOk, bOk, cOk, dOk;
        double initialConcentB = ui->conBLineEdit->text().toDouble(&bOk);
        double initialConcentC = ui->conCLineEdit->text().toDouble(&cOk);
        double initialConcentD = ui->conDLineEdit->text().toDouble(&dOk);

        if (!bOk || !cOk || !dOk) {
            throw ValidationError("Ошибка: некорректные начальные значения концентраций.", 1006);
        }

        if (initialConcentB < 0 || initialConcentC < 0 || initialConcentD < 0) {
            throw ValidationError("Ошибка: начальные концентрации не могут быть отрицательными.", 1007);
        }

        MainWindow::model -> initializeData(times, concentrations, initialConcentB, initialConcentC, initialConcentD);
        MainWindow::model -> calculateReactionParameters();
        QVector<double> concA = MainWindow::model -> getConcentrationsA();
        QVector<double> concB = MainWindow::model -> getConcentrationsB();
        QVector<double> concC = MainWindow::model -> getConcentrationsC();
        QVector<double> concD =  MainWindow::model -> getConcentrationsD();

        drawReactionGraphs(times, concA, concB, concC, concD);

        ReactionParametersDialog* paramsDialog = new ReactionParametersDialog(
            MainWindow::model->getRateConstant(),
            MainWindow::model->getReactionOrder(),
            MainWindow::model->getDispersion(),
            MainWindow::model->getCorrelation(),
            this
            );
        paramsDialog->exec(); // Ожидаем закрытия диалога

    } catch (const ValidationError& e) {
        QMessageBox::warning(this, "Ошибка", QString::fromStdString(e.what()));
    } catch (const ModelException& e) {
        QMessageBox::warning(this, "Ошибка", QString::fromStdString(e.what()));
    }
}


void MainWindow::drawReactionGraphs(const QVector<double>& times,
                                    const QVector<double>& concA,
                                    const QVector<double>& concB,
                                    const QVector<double>& concC,
                                    const QVector<double>& concD)
{
    // Создаём серии для концентраций A, B, C и D
    QLineSeries* seriesA = new QLineSeries();
    QLineSeries* seriesB = new QLineSeries();
    QLineSeries* seriesC = new QLineSeries();
    QLineSeries* seriesD = new QLineSeries();

    // Заполняем серии данными
    for (int i = 0; i < times.size(); ++i) {
        seriesA->append(times[i], concA[i]);
        seriesB->append(times[i], concB[i]);
        seriesC->append(times[i], concC[i]);
        seriesD->append(times[i], concD[i]);
    }

    // Устанавливаем имена и цвета для серий
    seriesA->setName("Концентрация A");
    seriesB->setName("Концентрация B");
    seriesC->setName("Концентрация C");
    seriesD->setName("Концентрация D");

    seriesA->setColor(Qt::red);
    seriesB->setColor(Qt::green);
    seriesC->setColor(Qt::blue);
    seriesD->setColor(Qt::magenta);

    // Создаём QChart и добавляем в него серии
    QChart* chart = new QChart();
    chart->addSeries(seriesA);
    chart->addSeries(seriesB);
    chart->addSeries(seriesC);
    chart->addSeries(seriesD);
    chart->setTitle("Экспериментальные точки и апроксимационная кривая");

    // Создаём оси X и Y
    QValueAxis* axisX = new QValueAxis();
    axisX->setTitleText("Время (t)");
    axisX->setLabelFormat("%.2f");   // Формат чисел на оси X


    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Концентрация (С)");
    axisY->setLabelFormat("%.2f");   // Формат чисел на оси Y

    // Добавляем оси к графику
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Привязываем серии данных к осям
    seriesA->attachAxis(axisX);
    seriesA->attachAxis(axisY);

    seriesB->attachAxis(axisX);
    seriesB->attachAxis(axisY);

    seriesC->attachAxis(axisX);
    seriesC->attachAxis(axisY);

    seriesD->attachAxis(axisX);
    seriesD->attachAxis(axisY);


    // Создаём QChartView для отображения QChart
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // Улучшение качества отрисовки

    chartView->setFixedSize(ui->graphicsView->viewport()->size());

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->addWidget(chartView);
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}
