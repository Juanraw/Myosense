#include "myoread.hpp"

MyoRead::MyoRead(QObject *parent)

    : QObject{parent}, running(false), hub(nullptr), myo(nullptr)
{}

MyoRead::~MyoRead() {

    stop();

}

void MyoRead::run() {

    hub = new myo::Hub("com.myosense.qt.myo");
    myo = hub->waitForMyo(10000);
    if (!myo) return;

    myo->setStreamEmg(myo::Myo::streamEmgEnabled);
    hub->addListener(this);

    running = true;
    while(running) {
        hub->run(1000 / 50);
        QThread::msleep(5);
    }

    emit finished();
}

void MyoRead::stop() {

    running = false;

}

void MyoRead::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) {

    QVector<qint8> data(8);
    for (int i = 0; i < 8; ++i) {
        data[i] = emg[i];
    }
    emit emgDataReceived(static_cast<qint64>(timestamp), data);

}

void MyoRead::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {

    emit poseReceived(static_cast<qint64>(timestamp), pose);

}

void MyoRead::onLock(myo::Myo* myo, uint64_t timestamp){

    isUnlocked = false;

    emit LockReceived(static_cast<qint64>(timestamp), isUnlocked);

}

void MyoRead::onUnlock(myo::Myo* myo, uint64_t timestamp){

    isUnlocked = true;

    emit LockReceived(static_cast<qint64>(timestamp), isUnlocked);

}

void MyoRead::setlock(){

    if(myo){
        myo->lock();
    }

}

void MyoRead::setUnlock(myo::Myo::UnlockType type){

    if(myo){
        myo->unlock(type);
    }

}
