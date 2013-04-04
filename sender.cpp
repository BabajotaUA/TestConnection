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

void Sender::download(const QByteArray &part)
{
    //request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setRawHeader("Range",part);
    emit newReply(networkManager.get(request));
}


