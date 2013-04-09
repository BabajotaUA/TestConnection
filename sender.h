#pragma once

#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

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
    void download(const QByteArray &part);
    
private slots:

private:
    QNetworkAccessManager networkManager;
    QNetworkRequest request;
    
};
