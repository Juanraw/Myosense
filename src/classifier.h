#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <QVector>

class Classifier{

public:
    Classifier();
    int predict(const QVector<double>& feature);

private:
    QVector<QVector<double>> Xtrain;
    QVector<int> Ytrain;
    QVector<double> mu = {0.7065, 0.3280, 0.2862, 0.2448, 0.1411};
    QVector<double> sigma = {0.2102, 0.0951, 0.2347, 0.2126, 0.1791};
    int k = 100;

    bool loadCSV(const QString& filename, QVector<QVector<double>>& data);
    bool loadCSVLabels(const QString& filename, QVector<int>& labels);
    double euclideanDistance(const QVector<double>& a, const QVector<double>& b);
};

#endif // CLASSIFIER_H
