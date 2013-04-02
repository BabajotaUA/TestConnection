#include "sender.h"
#include <QDebug>

Sender::Sender(const QUrl &fileURL, QObject *parent) :
    QObject(parent)
{
    request = QNetworkRequest(fileURL);
}

Sender::~Sender()
{
}

void Sender::getDownloadInfo()
{
    emit newReply(networkManager.head(request));
}

void Sender::startDownloading()
{
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    emit newReply(networkManager.get(request));
}


void Sender::startParts(const QList<QString> &parts)
{
    foreach(QString part, parts)
    {
        request.setRawHeader("Range",part.toUtf8());
        networkManager.get(request);
    }
}


