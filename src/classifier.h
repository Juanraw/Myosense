#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <QVector>

class Classifier{

public:
    Classifier();
    int predict(const QVector<double>& feature);

private:
    QVector<double> Beta;
    double Bias;
};

#endif // CLASSIFIER_H
