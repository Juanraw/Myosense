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
    void setlock();
    void setUnlock(myo::Myo::UnlockType type);


signals:

    void emgDataReceived(quint64 timestamp, QVector<qint8> emg);
    void poseReceived(quint64 timestamp, myo::Pose pose);
    void LockReceived(quint64 timestamp, bool isUnlocked);
    void finished();

protected:

    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) override;
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) override;
    void onLock(myo::Myo* myo, uint64_t timestamp) override;
    void onUnlock(myo::Myo* myo, uint64_t timestamp) override;

private:
    bool running;
    myo::Hub* hub;
    myo::Myo* myo;
    bool isUnlocked;

};

#endif // MYOREAD_HPP
