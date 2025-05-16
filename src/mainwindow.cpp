#define NOMINMAX
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "myoread.hpp"
#include "classifier.h"
#include <cmath>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QtMath>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dataTimer(new QTimer(this))
    , myoReader(new MyoRead())
    , classifier(new Classifier())

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

    //Adquisition Graph
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

    //Visualization Grapjs
    ui->EmgGraph1->addGraph();
    ui->EmgGraph1->xAxis2->setLabel("EMG 1");
    ui->EmgGraph1->xAxis->setLabel("Time");
    ui->EmgGraph1->yAxis->setLabel("F(t)");
    ui->EmgGraph1->graph(0)->setPen(QPen(Qt::blue));

    ui->EmgGraph2->addGraph();
    ui->EmgGraph2->xAxis2->setLabel("EMG 2");
    ui->EmgGraph2->xAxis->setLabel("Time");
    ui->EmgGraph2->yAxis->setLabel("F(t)");
    ui->EmgGraph2->graph(0)->setPen(QPen(Qt::blue));

    ui->EmgGraph3->addGraph();
    ui->EmgGraph3->xAxis2->setLabel("EMG 3");
    ui->EmgGraph3->xAxis->setLabel("Time");
    ui->EmgGraph3->yAxis->setLabel("F(t)");
    ui->EmgGraph3->graph(0)->setPen(QPen(Qt::blue));

    ui->EmgGraph4->addGraph();
    ui->EmgGraph4->xAxis2->setLabel("EMG 4");
    ui->EmgGraph4->xAxis->setLabel("Time");
    ui->EmgGraph4->yAxis->setLabel("F(t)");
    ui->EmgGraph4->graph(0)->setPen(QPen(Qt::blue));

    ui->EmgGraph5->addGraph();
    ui->EmgGraph5->xAxis2->setLabel("EMG 5");
    ui->EmgGraph5->xAxis->setLabel("Time");
    ui->EmgGraph5->yAxis->setLabel("F(t)");
    ui->EmgGraph5->graph(0)->setPen(QPen(Qt::blue));

    ui->EmgGraph6->addGraph();
    ui->EmgGraph6->xAxis2->setLabel("EMG 6");
    ui->EmgGraph6->xAxis->setLabel("Time");
    ui->EmgGraph6->yAxis->setLabel("F(t)");
    ui->EmgGraph6->graph(0)->setPen(QPen(Qt::blue));

    ui->EmgGraph7->addGraph();
    ui->EmgGraph7->xAxis2->setLabel("EMG 7");
    ui->EmgGraph7->xAxis->setLabel("Time");
    ui->EmgGraph7->yAxis->setLabel("F(t)");
    ui->EmgGraph7->graph(0)->setPen(QPen(Qt::blue));

    ui->EmgGraph8->addGraph();
    ui->EmgGraph8->xAxis2->setLabel("EMG 8");
    ui->EmgGraph8->xAxis->setLabel("Time");
    ui->EmgGraph8->yAxis->setLabel("F(t)");
    ui->EmgGraph8->graph(0)->setPen(QPen(Qt::blue));

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

void MainWindow::SaveData(const QString patientFolder){

    int Datasize = emg1Save.size();

    QString Name     = ui->NameLine->text();
    QString LastName = ui->LastNLine->text();
    QString Age      = ui->AgeLine->text();
    QString Weight   = ui->WeigthLine->text();
    QString Gender   = ui->GenderBox->currentText();
    QString Date     = ui->DateEdit->text();
    QString Frec     = ui->FrecBox->currentText();
    QString pathEmgTxt, pathTimeTxt, pathGeneral;

    if (ui->FatigeButton->isChecked()){
        pathEmgTxt = patientFolder + "/" + "Fatigue.txt";
        pathTimeTxt = patientFolder + "/" +  "Time-Fatigue.txt";
    } else {
        pathEmgTxt = patientFolder + "/" + "No-Fatigue.txt";
        pathTimeTxt = patientFolder + "/" + "No-Time-Fatigue.txt";
    }

    pathGeneral = patientFolder + "/" + "General.txt";

    QFile General(pathGeneral);
    QFile file(pathEmgTxt);

    if (!General.exists()){
        if (General.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&General);

            out << "Name: " << Name << "\n"
                << "Last Name: " << LastName << "\n"
                << "Age: " << Age << "\n"
                << "Weigth: " << Weight << "\n"
                << "Gender: " << Gender << "\n"
                << "Date: " << Date << "\n"
                << "Training Frecuency; " << Frec << "\n";

            General.close();
            qDebug() << "Save General Info into: " << patientFolder;
        } else {
            qDebug() << "Could't Save General Info";
        }
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);

        for(int i = 0; i < Datasize; i++){
            out << emg1Save[i] << "\t"
                << emg2Save[i] << "\t"
                << emg3Save[i] << "\t"
                << emg4Save[i] << "\t"
                << emg5Save[i] << "\t"
                << emg6Save[i] << "\t"
                << emg7Save[i] << "\t"
                << emg8Save[i] << "\n";
        }

        file.close();
        qDebug() << "Save Data into: " << patientFolder;
    } else {
        qDebug() << "Could't Save The Data";
    }

}

double MainWindow::Entropy(const QVector<double>& signal)
{

    const int numBins = 256;

    if(signal.isEmpty()) return 0.0;

    double minVal = *std::min_element(signal.begin(), signal.end());
    double maxVal = *std::max_element(signal.begin(), signal.end());

    if(qFuzzyCompare(minVal, maxVal)){
        return 0.0;
    }

    QVector<int> hist(numBins,0);
    double binWith = (maxVal - minVal) / numBins;

    for (double value : signal){
        int binIndex = static_cast<int>((value - minVal) / binWith);
        if (binIndex == numBins) binIndex = numBins - 1;
        hist[binIndex]++;
    }

    double Shannon = 0.0;
    int totalSamples = signal.size();

    for (int count : hist){
        if (count > 0){
            double p = static_cast<double>(count) / totalSamples;
            Shannon += p * (qLn(p) /qLn(2));
        }
    }

    return -Shannon;

}

double MainWindow::dEntropy(const QVector<double>& entropy){

    if(entropy.isEmpty()) return 0.0;

    return (entropy[0] - (entropy[entropy.size() - 1])) / W;

}

double MainWindow::MeanAbsoluteValue(const QVector<double>& signal){

    if(signal.isEmpty()) return 0.0;

    double sumAbs = 0.0;

    for(double value : signal){
        sumAbs += qAbs(value);
    }

    return sumAbs / signal.size();

}

double MainWindow::RootMeanSquare(const QVector<double>& signal){

    if(signal.isEmpty()) return 0.0;

    double sumSquares = 0.0;

    for (double value : signal){
        sumSquares += value * value;
    }

    return qSqrt(sumSquares / signal.size());

}

double MainWindow::Variance(const QVector<double>& signal){

    if (signal.size() < 2) return 0.0;

    double mean = 0.0;
    for (double value : signal){
        mean += value;
    }
    mean /= signal.size();

    double sumSquiereDiff = 0.0;
    for (double value : signal){
        double diff = value - mean;
        sumSquiereDiff += diff * diff;
    }

    return sumSquiereDiff / (signal.size() - 1);

}

Features MainWindow::DataProcessing(const QVector<double>& signal)
{

    Features result;

    int signalLength = signal.size();
    QVector<double> subSignal;
    QVector<double> row;

    for (int i = 0; i <= signalLength; i++){
        subSignal = signal.mid(i,W);

        result.h.append(Entropy(subSignal));
        result.dh.append(dEntropy(result.h));
        result.mav.append(MeanAbsoluteValue(subSignal));
        result.rms.append(RootMeanSquare(subSignal));
        result.var.append(Variance(subSignal));
    }

    auto normalize = [](QVector<double>& feature) {
        if (feature.isEmpty()) return;

        double minVal = *std::min_element(feature.constBegin(), feature.constEnd());
        double maxVal = *std::max_element(feature.constBegin(), feature.constEnd());
        double range = maxVal - minVal;
        if (qFuzzyIsNull(range)) range = 1.0; // evitar división por cero

        for (int i = 0; i < feature.size(); ++i) {
            feature[i] = (feature[i] - minVal) / range;
        }
    };

    normalize(result.h);
    normalize(result.dh);
    normalize(result.rms);
    normalize(result.mav);
    normalize(result.var);

    for (int i = 0; i <= signalLength; i++){
        row = {result.h[i], result.dh[i], result.rms[i], result.mav[i], result.var[i]};
        //row = {result.dh[i], result.rms[i], result.mav[i], result.var[i]};

        result.pred.append(classifier->predict(row));
    }

    return result;

}

void MainWindow::on_DataButton_clicked()
{

    if (ButtonValue){
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
    QString pathName = ui->NameLine->text();
    QString pathLastN = ui->LastNLine->text();
    QString FileName = pathLastN + "_" + pathName;

    patientFolder = basePath + "/" + FileName;

    QDir dir;
    if (!dir.exists(patientFolder)) {
        if (dir.mkpath(patientFolder)) {
            qDebug() << "Created Folder:" << patientFolder;
        } else {
            qDebug() << "Error in the Folder Creation:" << patientFolder;
        }
    }

    SaveData(patientFolder);

}


void MainWindow::on_ViewButton_clicked()
{

    ui->EmgGraph1->graph(0)->setData(TimeSave, emg1Save);
    ui->EmgGraph1->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph1->replot();

    ui->EmgGraph2->graph(0)->setData(TimeSave, emg2Save);
    ui->EmgGraph2->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph2->replot();

    ui->EmgGraph3->graph(0)->setData(TimeSave, emg3Save);
    ui->EmgGraph3->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph3->replot();

    ui->EmgGraph4->graph(0)->setData(TimeSave, emg4Save);
    ui->EmgGraph4->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph4->replot();

    ui->EmgGraph5->graph(0)->setData(TimeSave, emg5Save);
    ui->EmgGraph5->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph5->replot();

    ui->EmgGraph6->graph(0)->setData(TimeSave, emg6Save);
    ui->EmgGraph6->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph6->replot();

    ui->EmgGraph7->graph(0)->setData(TimeSave, emg7Save);
    ui->EmgGraph7->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph7->replot();

    ui->EmgGraph8->graph(0)->setData(TimeSave, emg8Save);
    ui->EmgGraph8->yAxis->setRange(-1.2,1.2);
    ui->EmgGraph8->replot();

}


void MainWindow::on_ProcessButton_clicked()
{

    QString HFile = patientFolder + "/" + "Hfile.txt";
    QString dHFile = patientFolder + "/" + "dHfile.txt";
    QString RMSFile = patientFolder + "/" + "RMSfile.txt";
    QString MAVFile = patientFolder + "/" + "MAVfile.txt";
    QString VARFile = patientFolder + "/" + "VARfile.txt";
    QString PredFile = patientFolder + "/" + "PREDICTIONfile.txt";

    QFile fileH(HFile);
    QFile filedH(dHFile);
    QFile fileRMS(RMSFile);
    QFile fileMAV(MAVFile);
    QFile fileVAR(VARFile);
    QFile filePred(PredFile);

    Features features1 = DataProcessing(emg1Save);
    Features features2 = DataProcessing(emg2Save);
    Features features3 = DataProcessing(emg3Save);
    Features features4 = DataProcessing(emg4Save);
    Features features5 = DataProcessing(emg5Save);
    Features features6 = DataProcessing(emg6Save);
    Features features7 = DataProcessing(emg7Save);
    Features features8 = DataProcessing(emg8Save);

    H1 = features1.h;
    dH1 = features1.dh;
    MAV1 = features1.mav;
    RMS1 = features1.rms;
    VAR1 = features1.var;
    Pred1 = features1.pred;

    H2 = features2.h;
    dH2 = features2.dh;
    MAV2 = features2.mav;
    RMS2 = features2.rms;
    VAR2 = features2.var;
    Pred2 = features2.pred;

    H3 = features3.h;
    dH3 = features3.dh;
    MAV3 = features3.mav;
    RMS3 = features3.rms;
    VAR3 = features3.var;
    Pred3 = features3.pred;

    H4 = features4.h;
    dH4 = features4.dh;
    MAV4 = features4.mav;
    RMS4 = features4.rms;
    VAR4 = features4.var;
    Pred4 = features4.pred;

    H5 = features5.h;
    dH5 = features5.dh;
    MAV5 = features5.mav;
    RMS5 = features5.rms;
    VAR5 = features5.var;
    Pred5 = features5.pred;

    H6 = features6.h;
    dH6 = features6.dh;
    MAV6 = features6.mav;
    RMS6 = features6.rms;
    VAR6 = features6.var;
    Pred6 = features6.pred;

    H7 = features7.h;
    dH7 = features7.dh;
    MAV7 = features7.mav;
    RMS7 = features7.rms;
    VAR7 = features7.var;
    Pred7 = features7.pred;

    H8 = features8.h;
    dH8 = features8.dh;
    MAV8 = features8.mav;
    RMS8 = features8.rms;
    VAR8 = features8.var;
    Pred8 = features8.pred;

    int Datasize = H1.size();

    if (Datasize == 0) {
        QMessageBox::warning(this, "Error", "Los vectores de características están vacíos.");
        return;
    }

    if (fileH.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&fileH);

        for (int i = 0; i < Datasize; i++){
            out << H1[i] << "\t"
                << H2[i] << "\t"
                << H3[i] << "\t"
                << H4[i] << "\t"
                << H5[i] << "\t"
                << H6[i] << "\t"
                << H7[i] << "\t"
                << H8[i] << "\n";
        }

        fileH.close();
        qDebug() << "Save H Data into: " << patientFolder;

    } else {
        qDebug() << "Could't Save The H Data";
    }

    if (filedH.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&filedH);

        for (int i = 0; i < Datasize; i++){
            out << dH1[i] << "\t"
                << dH2[i] << "\t"
                << dH3[i] << "\t"
                << dH4[i] << "\t"
                << dH5[i] << "\t"
                << dH6[i] << "\t"
                << dH7[i] << "\t"
                << dH8[i] << "\n";
        }

        fileH.close();
        qDebug() << "Save dH Data into: " << patientFolder;

    } else {
        qDebug() << "Could't Save The dH Data";
    }

    if (fileMAV.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&fileMAV);

        for (int i = 0; i < Datasize; i++){
            out << MAV1[i] << "\t"
                << MAV2[i] << "\t"
                << MAV3[i] << "\t"
                << MAV4[i] << "\t"
                << MAV5[i] << "\t"
                << MAV6[i] << "\t"
                << MAV7[i] << "\t"
                << MAV8[i] << "\n";
        }

        fileH.close();
        qDebug() << "Save MAV Data into: " << patientFolder;

    } else {
        qDebug() << "Could't Save The MAV Data";
    }

    if (fileRMS.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&fileRMS);

        for (int i = 0; i < Datasize; i++){
            out << RMS1[i] << "\t"
                << RMS2[i] << "\t"
                << RMS3[i] << "\t"
                << RMS4[i] << "\t"
                << RMS5[i] << "\t"
                << RMS6[i] << "\t"
                << RMS7[i] << "\t"
                << RMS8[i] << "\n";
        }

        fileH.close();
        qDebug() << "Save RMS Data into: " << patientFolder;

    } else {
        qDebug() << "Could't Save The RMS Data";
    }

    if (fileVAR.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&fileVAR);

        for (int i = 0; i < Datasize; i++){
            out << VAR1[i] << "\t"
                << VAR2[i] << "\t"
                << VAR3[i] << "\t"
                << VAR4[i] << "\t"
                << VAR5[i] << "\t"
                << VAR6[i] << "\t"
                << VAR7[i] << "\t"
                << VAR8[i] << "\n";
        }

        fileH.close();
        qDebug() << "Save VAR Data into: " << patientFolder;

    } else {
        qDebug() << "Could't Save The VAR Data";
    }

    if (filePred.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&filePred);

        for (int i = 0; i < Datasize; i++){
            out << Pred1[i] << "\t"
                << Pred2[i] << "\t"
                << Pred3[i] << "\t"
                << Pred4[i] << "\t"
                << Pred5[i] << "\t"
                << Pred6[i] << "\t"
                << Pred7[i] << "\t"
                << Pred8[i] << "\n";
        }

        fileH.close();
        qDebug() << "Save PREDICTION Data into: " << patientFolder;

    } else {
        qDebug() << "Could't Save The PREDICTION Data";
    }


}


