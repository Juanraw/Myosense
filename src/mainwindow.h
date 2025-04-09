#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include "myoread.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onEmgDataReceived(qint64 timestamp, QVector<qint8> emg);

private:
    Ui::MainWindow *ui;
    QCustomPlot *customplot;
    QTimer *dataTimer;
    MyoRead* myoReader;

    QVector<double> Time;
    QVector<double> emg1;
    QVector<double> emg2;
    QVector<double> emg3;
    QVector<double> emg4;
    QVector<double> emg5;
    QVector<double> emg6;
    QVector<double> emg7;
    QVector<double> emg8;

    void setupPlot();
    void updatePlot();
    double x = 0.0;
};
#endif // MAINWINDOW_H
