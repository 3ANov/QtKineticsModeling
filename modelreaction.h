#ifndef MODELREACTION_H
#define MODELREACTION_H

#include <QString>
#include <QVector>


class ModelReaction
{
private:
    double concentB;
    double concentC;
    double concentD;
    QVector<double> inputConcentrationsA;
    QVector<double> concentrationsA;
    QVector<double> concentrationsB;
    QVector<double> concentrationsC;
    QVector<double> concentrationsD;
    QVector<double> times;
    int recordCount;

    double rateConstant;
    double reactionOrder;
    double dispersion;
    double correlation;

public:
    ModelReaction();

    void initializeData(
            const QVector<double>& timesData,
            const QVector<double>& concentrationsData,
            double initialConcentB,
            double initialConcentC,
            double initialConcentD
        )
    {
        times = timesData;
        inputConcentrationsA = concentrationsData;
        concentB = initialConcentB;
        concentC = initialConcentC;
        concentD = initialConcentD;
        recordCount = timesData.size();

        concentrationsA.clear();
        concentrationsB.clear();
        concentrationsC.clear();
        concentrationsD.clear();
    }

    double getRateConstant() const { return rateConstant; }
    double getReactionOrder() const { return reactionOrder; }
    double getDispersion() const { return dispersion; }
    double getCorrelation() const { return correlation; }
    QVector<double> getConcentrationsA() const { return concentrationsA; }
    QVector<double> getConcentrationsB() const { return concentrationsB; }
    QVector<double> getConcentrationsC() const { return concentrationsC; }
    QVector<double> getConcentrationsD() const { return concentrationsD; }
    QVector<double> getTimes() const { return times; }

    void calculateReactionParameters();

};

#endif // MODELREACTION_H
