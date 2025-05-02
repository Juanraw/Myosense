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
    void onPoseReceived(qint64 timestamp, myo::Pose pose);
    void onLockReceived(qint64 timestamp, bool isUnlock);
    void SaveData(const QString patientFolder);
    void on_DataButton_clicked();
    void on_SaveButton_clicked();

    void on_ViewButton_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QCustomPlot *customplot;
    QTimer *dataTimer;
    MyoRead* myoReader;

    QVector<double> Time, emg1, emg2, emg3, emg4, emg5, emg6, emg7, emg8;
    QVector<double> TimeSave, emg1Save, emg2Save, emg3Save, emg4Save, emg5Save, emg6Save, emg7Save, emg8Save;
    myo::Pose pose;
    bool isUnlock;
    bool ButtonValue = false;
    QString name;
    QString lastName;
    QString appPath = QCoreApplication::applicationDirPath();

    void setupPlot();
    void updatePlot();
    double x = 0.0;
};
#endif // MAINWINDOW_H
