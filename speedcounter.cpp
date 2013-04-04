#include "speedcounter.h"
#include <QDebug>

SpeedCounter::SpeedCounter(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(parent);
    connect(timer,SIGNAL(timeout()),SLOT(timeChanged()));
}

SpeedCounter::~SpeedCounter()
{
}

void SpeedCounter::start()
{
    seconds = 1;
    timer->start(1000);
}

void SpeedCounter::stop()
{
    timer->stop();
}

qreal SpeedCounter::bytesReciving(qint64 bytesReceived)
{
    auto result = bytesReceived/seconds;
    return result;
}


void SpeedCounter::timeChanged()
{
    ++seconds;
}
