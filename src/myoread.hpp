#ifndef MYOREAD_HPP
#define MYOREAD_HPP

#include <QObject>
#include <QThread>
#include <array>
#include <myo/myo.hpp>

class MyoRead : public QObject
{
    Q_OBJECT
public:
    explicit MyoRead(QObject *parent = nullptr);

signals:
};

#endif // MYOREAD_HPP
