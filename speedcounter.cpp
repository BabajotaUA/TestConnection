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

qreal SpeedCounter::bytesReciving(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesReceived == bytesTotal)
    {
        timer->stop();
        return 0;
    }

    auto result = bytesReceived/seconds;
    return result;
}


void SpeedCounter::timeChanged()
{
    ++seconds;
}
