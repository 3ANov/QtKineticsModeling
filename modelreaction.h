#ifndef MODELREACTION_H
#define MODELREACTION_H

#include <QString>
#include <QVector>


class ModelReaction
{
private:
    double rateConstant;
    double concentA;
    double concentB;
    double concentC;
    double concentD;
    QVector<double> concentrations;
    QVector<double> times;
    int recordCount;

public:
    ModelReaction();
};

#endif // MODELREACTION_H
