#include "exceptions.h"
#include "qdebug.h"
#include "modelreaction.h"
#include <QtMath>

ModelReaction::ModelReaction(){}


void ModelReaction::calculateReactionParameters() {
    // Проверка на количество точек
    if (times.size() < 2) {
        throw ModelException("Недостаточно данных для расчёта.");
    }

    // Инициализация начальных концентраций
    double ca = inputConcentrationsA[0];  // Начальная концентрация A из массива
    double cb = concentB;            // Начальная концентрация B
    double cc = concentC;            // Начальная концентрация C
    double cd = concentD;            // Начальная концентрация D
    dispersion = 0.0;

    // Переменные для расчёта параметров реакции
    QVector<double> x(times.size() - 1);
    QVector<double> y(times.size() - 1);
    double s1 = times.size() - 1;
    double s2 = 0.0, s3 = 0.0, s4 = 0.0, s5 = 0.0, s6 = 0.0;

    // Расчёт значений для логарифмического анализа
    for (int i = 0; i < times.size() - 1; ++i) {
        double t = times[i + 1] - times[i];
        double deltaCa = inputConcentrationsA[i] - inputConcentrationsA[i + 1];

        if (deltaCa <= 0) {
            throw ModelException("Ошибка в данных: разность концентраций A должна быть положительной.");
        }

        // Вычисление логарифмических значений
        y[i] = log(fabs(deltaCa/t));
        x[i] = log(inputConcentrationsA[i]);

        s2 += x[i];
        s3 += y[i];
        s4 += x[i] * x[i];
        s5 += x[i] * y[i];
        s6 += y[i] * y[i];
    }

    // Расчёт константы скорости (k), порядка реакции (n) и коэффициента корреляции (r)
    rateConstant = exp((s3 * s4 - s2 * s5) / (s1 * s4 - s2 * s2));
    reactionOrder = (s1 * s5 - s2 * s3) / (s1 * s4 - s2 * s2);
    correlation = (s1 * s5 - s2 * s3) / sqrt((s1 * s4 - s2 * s2) * (s1 * s6 - s3 * s3));


    // Очищаем массивы концентраций для сохранения новых данных
    concentrationsA.clear();
    concentrationsB.clear();
    concentrationsC.clear();
    concentrationsD.clear();

    // Добавляем начальные концентрации в массивы
    concentrationsA.append(inputConcentrationsA[0]);
    concentrationsB.append(cb);
    concentrationsC.append(cc);
    concentrationsD.append(cd);

    // Расчёт концентраций веществ A, B, C и D на основе разложения
    for (int i = 1; i < times.size(); ++i) { // Начинаем с 1, так как начальная концентрация уже добавлена
        double t = times[i] - times[i - 1];  // Интервал времени между текущим и предыдущим значением
        ca = concentrationsA.last();  // Текущая концентрация A из последнего добавленного значения

        // Обновляем концентрации B, C и D на основе реакции
        cb += rateConstant * pow(ca, reactionOrder) * t;
        cc += rateConstant * pow(ca, reactionOrder) * t;
        cd += rateConstant * pow(ca, reactionOrder) * t;
        ca -= rateConstant * pow(ca, reactionOrder) * t;


        if (ca < 0) {
            throw ModelException("Модель невозможно описать линейной регрессией.");
        }

        // Добавляем новые концентрации в массивы
        concentrationsA.append(ca);
        concentrationsB.append(cb);
        concentrationsC.append(cc);
        concentrationsD.append(cd);


        // Расчёт дисперсии для концентраций A
        dispersion += pow(inputConcentrationsA[i] - ca, 2);
    }


    qDebug() << "Расчёт завершён успешно.";
    qDebug() << "Константа скорости k:" << rateConstant;
    qDebug() << "Порядок реакции n:" << reactionOrder;
    qDebug() << "Коэффициент корреляции r:" << correlation;
    qDebug() << "Дисперсия:" << dispersion;
    qDebug() << "Концентрации A:" << concentrationsA;
    qDebug() << "Концентрации B:" << concentrationsB;
    qDebug() << "Концентрации C:" << concentrationsC;
    qDebug() << "Концентрации D:" << concentrationsD;

}




