#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "myoread.hpp"
#include <cmath>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QDir>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dataTimer(new QTimer(this))
    , myoReader(new MyoRead())

{

    ui->setupUi(this);
    setupPlot();

    QThread* myoThread = new QThread(this);
    myoReader->moveToThread(myoThread);
    connect(myoThread, &QThread::started, myoReader, &MyoRead::run);
    connect(myoThread, &QThread::finished, myoReader, &QObject::deleteLater);
    connect(myoThread, &QThread::finished, myoThread, &QObject::deleteLater);
    connect(this, &MainWindow::destroyed, myoReader, &MyoRead::stop);

    connect(myoReader, &MyoRead::emgDataReceived, this, &MainWindow::onEmgDataReceived);
    connect(myoReader, &MyoRead::poseReceived, this, &MainWindow::onPoseReceived);
    connect(myoReader, &MyoRead::LockReceived, this, &MainWindow::onLockReceived);

    connect(dataTimer, &QTimer::timeout, this, &MainWindow::updatePlot);

    myoThread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){

    if(myoReader){
        myoReader->setlock();
    }

    event->accept();
}

void MainWindow::onEmgDataReceived(qint64 timestamp, QVector<qint8> emg)
{

    x += 0.005;

    Time.append(x);
    TimeSave.append(x);

    if(emg.size() >= 3) {
        emg1.append(emg[0] / 128.0);
        emg2.append(emg[1] / 128.0);
        emg3.append(emg[2] / 128.0);
        emg4.append(emg[3] / 128.0);
        emg5.append(emg[4] / 128.0);
        emg6.append(emg[5] / 128.0);
        emg7.append(emg[6] / 128.0);
        emg8.append(emg[7] / 128.0);

        emg1Save.append(emg[0] / 128.0);
        emg2Save.append(emg[1] / 128.0);
        emg3Save.append(emg[2] / 128.0);
        emg4Save.append(emg[3] / 128.0);
        emg5Save.append(emg[4] / 128.0);
        emg6Save.append(emg[5] / 128.0);
        emg7Save.append(emg[6] / 128.0);
        emg8Save.append(emg[7] / 128.0);

    } else {
        emg1.append(0);
        emg2.append(0);
        emg3.append(0);
        emg4.append(0);
        emg5.append(0);
        emg6.append(0);
        emg7.append(0);
        emg8.append(0);
    }

    if(x > 10) {
        Time.removeFirst();
        emg1.removeFirst();
        emg2.removeFirst();
        emg3.removeFirst();
        emg4.removeFirst();
        emg5.removeFirst();
        emg6.removeFirst();
        emg7.removeFirst();
        emg8.removeFirst();
    }

}

void MainWindow::onPoseReceived(qint64 timestamp, myo::Pose pose){

    ui->RestLamp->setStyleSheet("background-color: red; border-radius: 10px; border: 1px solid black");
    ui->FistLamp->setStyleSheet("background-color: red; border-radius: 10px; border: 1px solid black");
    ui->SpreadLamp->setStyleSheet("background-color: red; border-radius: 10px; border: 1px solid black");
    ui->WaveinLamp->setStyleSheet("background-color: red; border-radius: 10px; border: 1px solid black");
    ui->WaveoutLamp->setStyleSheet("background-color: red; border-radius: 10px; border: 1px solid black");
    ui->TapLamp->setStyleSheet("background-color: red; border-radius: 10px; border: 1px solid black");

    switch (pose.type()) {
    case myo::Pose::rest:
        ui->RestLamp->setStyleSheet("background-color: green; border-radius: 10px; border: 1px solid black");
        break;
    case myo::Pose::fist:
        ui->FistLamp->setStyleSheet("background-color: green; border-radius: 10px; border: 1px solid black");
        break;
    case myo::Pose::fingersSpread:
        ui->SpreadLamp->setStyleSheet("background-color: green; border-radius: 10px; border: 1px solid black");
        break;
    case myo::Pose::waveIn:
        ui->WaveinLamp->setStyleSheet("background-color: green; border-radius: 10px; border: 1px solid black");
        break;
    case myo::Pose::waveOut:
        ui->WaveoutLamp->setStyleSheet("background-color: green; border-radius: 10px; border: 1px solid black");
        break;
    case myo::Pose::doubleTap:
        ui->TapLamp->setStyleSheet("background-color: green; border-radius: 10px; border: 1px solid black");
        break;
    default:
        break;
    }

}

void MainWindow::setupPlot()
{

    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->xAxis->setLabel("Time");
    ui->Realtimeplot->yAxis->setLabel("F(t)");
    ui->Realtimeplot->xAxis->setRange(0,10);
    ui->Realtimeplot->yAxis->setRange(-1.2,1.2);
    ui->Realtimeplot->graph(0)->setPen(QPen(Qt::blue));
    ui->Realtimeplot->graph(1)->setPen(QPen(Qt::black));
    ui->Realtimeplot->graph(2)->setPen(QPen(Qt::red));
    ui->Realtimeplot->graph(3)->setPen(QPen(Qt::cyan));
    ui->Realtimeplot->graph(4)->setPen(QPen(Qt::green));
    ui->Realtimeplot->graph(5)->setPen(QPen(Qt::yellow));
    ui->Realtimeplot->graph(6)->setPen(QPen(Qt::magenta));
    ui->Realtimeplot->graph(7)->setPen(QPen(Qt::darkBlue));

}

void MainWindow::updatePlot()
{

    ui->Realtimeplot->graph(0)->setData(Time,emg1);
    ui->Realtimeplot->graph(1)->setData(Time,emg2);
    ui->Realtimeplot->graph(2)->setData(Time,emg3);
    ui->Realtimeplot->graph(3)->setData(Time,emg4);
    ui->Realtimeplot->graph(4)->setData(Time,emg5);
    ui->Realtimeplot->graph(5)->setData(Time,emg6);
    ui->Realtimeplot->graph(6)->setData(Time,emg7);
    ui->Realtimeplot->graph(7)->setData(Time,emg8);
    ui->Realtimeplot->xAxis->setRange(x-10, x);
    ui->Realtimeplot->replot();

}

void MainWindow::onLockReceived(qint64 timestamp, bool isUnlock){

    if(isUnlock){
        ui->StreamingLamp->setStyleSheet("background-color: green");
    } else {
        ui->StreamingLamp->setStyleSheet("background-color: red");
    }

}

void MainWindow::SaveData(const QString finalPath){

    int Datasize = emg1Save.size();

    QFile file(finalPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);

        for(int i = 0; i < Datasize; i++){
            out <<emg1Save[i] << "\t"
                <<emg2Save[i] << "\t"
                <<emg3Save[i] << "\t"
                <<emg4Save[i] << "\t"
                <<emg5Save[i] << "\t"
                <<emg6Save[i] << "\t"
                <<emg7Save[i] << "\t"
                <<emg8Save[i] << "\n";
        }

        file.close();
        qDebug() << "Save Data into: " << finalPath;
    } else {
        qDebug() << "Could't Save The Data";
    }

}

void MainWindow::on_DataButton_clicked()
{

    if(ButtonValue){
        myoReader->setlock();
        ui->DataButton->setText("Start Data");
        ui->DataButton->setStyleSheet("background-color: red");
        dataTimer->stop();
    } else {
        myoReader->setUnlock(myo::Myo::unlockHold);
        ui->DataButton->setText("Stop Data");
        ui->DataButton->setStyleSheet("background-color: green");
        x = 0.0;
        dataTimer->start(50);

    }

    ButtonValue = !ButtonValue;

}


void MainWindow::on_SaveButton_clicked()
{

    QString basePath = appPath;
    QString pathEmgTxt, pathTimeTxt;

    if(ui->FatigeButton->isChecked()){
        pathEmgTxt = basePath + "/Fatigue.txt";
        pathTimeTxt = basePath + "Time-Fatigue.txt";
    } else {
        pathEmgTxt = basePath + "/No-Fatigue1.txt";
        pathTimeTxt = basePath + "No-Time-Fatigue.txt";
    }

    SaveData(pathEmgTxt);

}

