#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

class SpeedCounter : public QObject
{
    Q_OBJECT
public:
    explicit SpeedCounter(QObject *parent = nullptr);
    ~SpeedCounter();

    void start();
    void stop();
    qreal bytesReciving(qint64 bytesReceived);

private slots:
    void timeChanged();

private:
    QTimer *timer;
    int seconds;

};
