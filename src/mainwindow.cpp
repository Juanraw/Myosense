#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dataTimer(new QTimer(this)
)

{
    ui->setupUi(this);
    setupPlot();

    connect(dataTimer, &QTimer::timeout, this, &MainWindow::updatePlot);
    dataTimer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPlot()
{

    ui->Realtimeplot->addGraph();
    ui->Realtimeplot->xAxis->setLabel("Time");
    ui->Realtimeplot->yAxis->setLabel("F(t)");
    ui->Realtimeplot->xAxis->setRange(0,10);
    ui->Realtimeplot->yAxis->setRange(-1.2,1.2);
    ui->Realtimeplot->graph(0)->setPen(QPen(Qt::blue));

}

void MainWindow::updatePlot()
{

    static QVector<double> xData, yData;

    x += 0.05;
    xData.append(x);
    yData.append(std::sin(x * 2 * M_PI));

    if (x > 10) {
        xData.remove(0);
        yData.remove(0);
    }

    ui->Realtimeplot->graph(0)->setData(xData,yData);
    ui->Realtimeplot->xAxis->setRange(x-10, x);
    ui->Realtimeplot->replot();

}
