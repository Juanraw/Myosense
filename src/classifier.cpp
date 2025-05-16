#include "classifier.h"
#include <QtMath>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>

Classifier::Classifier()
{

    Beta = {-1.6920, -1.2074, -5.3975, 20.2174, -7.8668};
    Bias = -0.5626;

}



int Classifier::predict(const QVector<double>& features)
{

    double sum = 0.0;
    for (int i = 0; i < features.size(); i++) {
        sum += features[i] * Beta[i];
    }
    sum += Bias;

    return (sum >= 0.0) ? 1 : 0;

}
