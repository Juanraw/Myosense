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

    if (!loadCSV(QString(DATAPATH) + "Xtrain_normalized.csv", Xtrain))
        qWarning() << "Unable to charge Xtrain.csv from" << DATAPATH;

    if (!loadCSVLabels(QString(DATAPATH) + "Ytrain.csv", Ytrain))
        qWarning() << "Unable to charge Ytrain.csv from" << DATAPATH;
}

bool Classifier::loadCSV(const QString& filename, QVector<QVector<double>>& data) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    data.clear();

    if (!in.atEnd())
        in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        QStringList parts = line.split(',', Qt::SkipEmptyParts);
        QVector<double> row;
        for (const QString& value : parts)
            row.append(value.toDouble());

        data.append(row);
    }

    file.close();
    return true;
}

bool Classifier::loadCSVLabels(const QString& filename, QVector<int>& labels) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    labels.clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;
        labels.append(line.toInt());
    }

    file.close();
    return true;
}

double Classifier::euclideanDistance(const QVector<double>& a, const QVector<double>& b) {
    double sum = 0.0;
    for (int i = 0; i < a.size(); ++i) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return qSqrt(sum);
}

int Classifier::predict(const QVector<double>& feature)
{

    if (Xtrain.isEmpty() || Ytrain.isEmpty()) {
        qWarning() << "No hay datos cargados para predecir.";
        return -1;
    }

    QVector<double> normFeature;
    for (int i = 0; i < feature.size(); ++i) {
        double normValue = (feature[i] - mu[i]) / sigma[i];
        normFeature.append(normValue);
    }

    QVector<QPair<double, int>> distances;
    for (int i = 0; i < Xtrain.size(); ++i) {
        //QVector<double> normTrain;
        //for (int j = 0; j < Xtrain[i].size(); ++j) {
        //    double normVal = (Xtrain[i][j] - mu[j]) / sigma[j];
        //    normTrain.append(normVal);
        //}
        double dist = euclideanDistance(normFeature, Xtrain[i]);
        distances.append(qMakePair(dist, Ytrain[i]));
    }

    std::sort(distances.begin(), distances.end(),
              [](const QPair<double, int>& a, const QPair<double, int>& b) {
                  return a.first < b.first;
              });

    int count0 = 0;
    int count1 = 0;

    for (int i = 0; i < qMin(k, distances.size()); ++i) {
        if (distances[i].second == 0) count0++;
        else count1++;
    }

    return (count1 >= count0) ? 1 : 0;
}
