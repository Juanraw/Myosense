#pragma once

#ifndef MYOREAD_HPP
#define MYOREAD_HPP

#include <QObject>
#include <QThread>
#include <array>
#include <myo/myo.hpp>

class MyoRead : public QObject, public myo::DeviceListener
{
    Q_OBJECT
public:
    explicit MyoRead(QObject *parent = nullptr);
    ~MyoRead();
    void run();
    void stop();

signals:

    void emgDataReceived(quint64 timestamp, QVector<qint8> emg);
    void finished();

protected:

    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) override;

private:
    bool running;
    myo::Hub* hub;
    myo::Myo* myo;

};

#endif // MYOREAD_HPP
