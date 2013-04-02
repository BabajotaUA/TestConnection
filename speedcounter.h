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
    qreal bytesReciving(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void timeChanged();

private:
    QTimer *timer;
    int seconds;

};
