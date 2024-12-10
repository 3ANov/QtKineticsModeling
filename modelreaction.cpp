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
    double ca = concentrationsA[0];  // Начальная концентрация A из массива
    double cb = concentB;            // Начальная концентрация B
    double cc = concentC;            // Начальная концентрация C
    double cd = concentD;            // Начальная концентрация D

    // Переменные для расчёта параметров реакции
    QVector<double> x(times.size() - 1);
    QVector<double> y(times.size() - 1);
    double s1 = times.size() - 1;
    double s2 = 0.0, s3 = 0.0, s4 = 0.0, s5 = 0.0, s6 = 0.0;

    // Расчёт значений для логарифмического анализа
    for (int i = 0; i < times.size() - 1; ++i) {
        double t = times[i + 1] - times[i];
        double deltaCa = concentrationsA[i] - concentrationsA[i + 1];

        if (deltaCa <= 0) {
            throw ModelException("Ошибка в данных: разность концентраций A должна быть положительной.");
        }

        // Вычисление логарифмических значений
        y[i] = log(fabs(t / deltaCa));
        x[i] = log(times[i]);

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
    concentrationsB.clear();
    concentrationsC.clear();
    concentrationsD.clear();

    // Добавляем начальные концентрации в массивы
    concentrationsB.append(cb);
    concentrationsC.append(cc);
    concentrationsD.append(cd);

    // Расчёт концентраций веществ B, C и D на основе разложения A
    for (int i = 0; i < times.size() - 1; ++i) {
        double t = times[i + 1] - times[i];

        // Обновляем концентрации B, C и D на основе реакции
        cb += rateConstant * pow(ca, reactionOrder) * t;
        cc += rateConstant * pow(ca, reactionOrder) * t;
        cd += rateConstant * pow(ca, reactionOrder) * t;

        // Добавляем новые концентрации в массивы
        concentrationsB.append(cb);
        concentrationsC.append(cc);
        concentrationsD.append(cd);
    }

    // Расчёт дисперсии для концентраций A
    dispersion = 0.0;
    for (int i = 0; i < times.size(); ++i) {
        dispersion += pow(concentrationsA[i] - ca, 2);
    }
    dispersion /= times.size();

    qDebug() << "Расчёт завершён успешно.";
    qDebug() << "Константа скорости k:" << rateConstant;
    qDebug() << "Порядок реакции n:" << reactionOrder;
    qDebug() << "Коэффициент корреляции r:" << correlation;
    qDebug() << "Дисперсия:" << dispersion;
}




