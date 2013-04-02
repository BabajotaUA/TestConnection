#pragma once

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

class Reciver : public QObject
{
    Q_OBJECT
public:
    explicit Reciver(QNetworkReply *reply, QObject *parent = 0);
    
signals:
    
public slots:
    
};
