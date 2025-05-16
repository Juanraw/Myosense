#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include "myoread.hpp"
#include "classifier.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Features{
    QVector<double> h;
    QVector<double> dh;
    QVector<double> mav;
    QVector<double> rms;
    QVector<double> var;
    QVector<double> pred;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onEmgDataReceived(qint64 timestamp, QVector<qint8> emg);
    void onPoseReceived(qint64 timestamp, myo::Pose pose);
    void onLockReceived(qint64 timestamp, bool isUnlock);

    void SaveData(const QString patientFolder);

    double Entropy(const QVector<double>& signal);
    double dEntropy(const QVector<double>& entropy);
    double MeanAbsoluteValue(const QVector<double>& signal);
    double RootMeanSquare(const QVector<double>& signal);
    double Variance(const QVector<double>& signal);

    Features DataProcessing(const QVector<double>& signal);

    void on_DataButton_clicked();
    void on_SaveButton_clicked();
    void on_ViewButton_clicked();

    void on_ProcessButton_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QCustomPlot *customplot;
    QTimer *dataTimer;
    MyoRead* myoReader;
    Classifier* classifier;

    QVector<double> Time, emg1, emg2, emg3, emg4, emg5, emg6, emg7, emg8;
    QVector<double> TimeSave, emg1Save, emg2Save, emg3Save, emg4Save, emg5Save, emg6Save, emg7Save, emg8Save;

    QVector<double> H1, H2, H3, H4, H5, H6, H7, H8;
    QVector<double> dH1, dH2, dH3, dH4, dH5, dH6, dH7, dH8;
    QVector<double> MAV1, MAV2, MAV3, MAV4, MAV5, MAV6, MAV7, MAV8;
    QVector<double> RMS1, RMS2, RMS3, RMS4, RMS5, RMS6, RMS7, RMS8;
    QVector<double> VAR1, VAR2, VAR3, VAR4, VAR5, VAR6, VAR7, VAR8;
    QVector<double> Pred1, Pred2, Pred3, Pred4, Pred5, Pred6, Pred7, Pred8;

    myo::Pose pose;
    bool isUnlock;
    bool ButtonValue = false;
    QString name;
    QString lastName;
    QString appPath = QCoreApplication::applicationDirPath();
    QString patientFolder;


    void setupPlot();
    void updatePlot();
    double x = 0.0;
    int W = 80;
};
#endif // MAINWINDOW_H
