#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(const QUrl &fileURL, QObject *parent = nullptr);
    virtual ~Sender();

    void getDownloadInfo();
    
signals:
    void newReply(QNetworkReply*);

public slots:
    void startDownloading();
    
private slots:

    void startParts(const QList<QString> &parts);

private:
    QNetworkAccessManager networkManager;
    QNetworkRequest request;
    
};
