#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "myoread.hpp"
#include <cmath>


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

    connect(dataTimer, &QTimer::timeout, this, &MainWindow::updatePlot);


    dataTimer->start(50);

    myoThread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onEmgDataReceived(qint64 timestamp, QVector<qint8> emg)
{

    x += 0.005;

    Time.append(x);

    if(emg.size() >= 3) {
        emg1.append(emg[0] / 128.0);
        emg2.append(emg[1] / 128.0);
        emg3.append(emg[2] / 128.0);
        emg4.append(emg[3] / 128.0);
        emg5.append(emg[4] / 128.0);
        emg6.append(emg[5] / 128.0);
        emg7.append(emg[6] / 128.0);
        emg8.append(emg[7] / 128.0);

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
    qDebug() << "Pose recibida en " << timestamp << ": " << QString::fromStdString(pose.toString());
    this->pose = pose;
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



