#include "exceptions.h"
#include "qdebug.h"
#include "modelreaction.h"
#include <QtMath>

ModelReaction::ModelReaction(){}

void ModelReaction::calculateReactionParameters()
{
    // Проверка на количество точек
    if (times.size() < 2) {
        throw ModelException("Недостаточно данных для расчёта.");
    }

    // Примерные переменные для расчётов
    QVector<double> x(times.size() - 1);
    QVector<double> y(times.size() - 1);

    double s1 = 0.0, s2 = 0.0, s3 = 0.0, s4 = 0.0, s5 = 0.0, s6 = 0.0;
    double k = 0.0, n = 0.0, r = 0.0;

    // Проверка наличия начальных концентраций A
    if (concentrationsA.isEmpty()) {
        throw ModelException("Ошибка: Концентрации A не заданы.");
    }

    double ca = concentrationsA[0]; // Начальная концентрация A
    double cb = concentB;
    double cc = concentC;
    double cd = concentD;

    // Вывод начальных концентраций для отладки
    qDebug() << "Начальная концентрация A: " << ca;
    qDebug() << "Начальная концентрация B: " << cb;
    qDebug() << "Начальная концентрация C: " << cc;
    qDebug() << "Начальная концентрация D: " << cd;

    // Начальные концентрации должны быть добавлены в массивы
    concentrationsB.clear();
    concentrationsC.clear();
    concentrationsD.clear();
    concentrationsB.append(cb);
    concentrationsC.append(cc);
    concentrationsD.append(cd);

    // Расчёт значений x и y для модели
    for (int i = 0; i < times.size() - 1; ++i) {
        // Проверка на наличие данных для концентрации A в массиве
        if (i + 1 >= concentrationsA.size()) {
            throw ModelException("Ошибка: недостаточно данных в концентрациях A.");
        }

        // Вычисление логарифмических значений
        y[i] = log(fabs(times[i + 1] - times[i]) / (concentrationsA[i + 1] - concentrationsA[i]));
        x[i] = log(times[i]);

        s2 += x[i];
        s3 += y[i];
        s4 += x[i] * x[i];
        s5 += x[i] * y[i];
        s6 += y[i] * y[i];

        // Вычисление концентраций веществ B, C, D на основе уравнений реакции
        double t = times[i + 1] - times[i];

        // Обновляем концентрации веществ B, C и D
        cb += k * pow(ca, n) * t;
        cc += k * 2 * pow(ca, n) * t;
        cd += k * 3 * pow(ca, n) * t;

        // Добавляем новые концентрации в массивы
        concentrationsA.append(ca);
        concentrationsB.append(cb);
        concentrationsC.append(cc);
        concentrationsD.append(cd);
    }

    // Расчёт константы скорости (k), порядка реакции (n) и коэффициента корреляции (r)
    k = exp((s3 * s4 - s2 * s5) / (s1 * s4 - s2 * s2));
    n = (s1 * s5 - s2 * s3) / (s1 * s4 - s2 * s2);
    r = (s1 * s5 - s2 * s3) / sqrt((s1 * s4 - s2 * s2) * (s1 * s6 - s3 * s3));

    // Сохранение результатов
    rateConstant = k;
    reactionOrder = n;
    correlation = r;

    // Примерное вычисление дисперсии
    for (int i = 0; i < times.size(); ++i) {
        dispersion += pow(concentrationsA[i] - rateConstant, 2);
    }
    dispersion /= times.size();

    qDebug() << "отработал расчёт";
}


