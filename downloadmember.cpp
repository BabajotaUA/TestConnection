#include "downloadmember.h"
#include <QDebug>
#include <QStringList>

DownloadMember::DownloadMember(const QUrl &source, const QString &destination, QObject *parent) :
    QObject(parent)
{
    fileName = source.path().split('/').last();
    fileDestination = destination;
    partSize = 2000000;
    maxFlows = 5;
    fileSize = 0;
    state = 0;

    sender = QSharedPointer<Sender>(new Sender(source,this));
    fileSaver = QSharedPointer<FileSaver>(new FileSaver(destination,this));
    speedCounter = QSharedPointer<SpeedCounter>(new SpeedCounter(this));

    connect(sender.data(),SIGNAL(newReply(QNetworkReply*)),SLOT(replyReciving(QNetworkReply*)));

    sender->getDownloadInfo();

    qDebug() << "(DownloadMember)Initialisation - completted";
}

DownloadMember::~DownloadMember()
{
}

qint8 DownloadMember::getState() const
{
    return state;
}

void DownloadMember::startDownloading()
{
    qDebug() << parts[0] << "-" << parts[1]-1;
    sender->download("bytes=" + QByteArray::number(parts[0]) + "-" + QByteArray::number(parts[1]-1));
    speedCounter->start();
    state = 2;
}

void DownloadMember::resumeDownloading()
{
    state = 2;
}

void DownloadMember::pauseDownloading()
{
    state = 3;
}

void DownloadMember::cancelDownloading()
{
    state = 0;
    prepareDownload();
}


void DownloadMember::splitParts(const qint64 &minSplitSize, const qint64 &startByte)
{
    for (auto endByte = startByte; endByte < fileSize; endByte += minSplitSize)
        parts.append(endByte);
    parts.append(fileSize);
}

void DownloadMember::prepareDownload()
{
    state = 1;
    fileSize = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    splitParts(partSize);
    fileSaver->prepareFile(parts);
    emit downloadIsReadyToStart();
}

void DownloadMember::saveDataAndContinue()
{
    speedCounter->stop();
    parts.removeFirst();
    fileSaver->savePart(reply->readAll());

}

void DownloadMember::replyReciving(QNetworkReply *newReply)
{
    reply = QSharedPointer<QNetworkReply>( newReply );
    connect(reply.data(),SIGNAL(finished()),SLOT(replyRecivingFinished()));

    if (reply->operation() == QNetworkAccessManager::GetOperation)
        connect(reply.data(),SIGNAL(downloadProgress(qint64,qint64)),SLOT(replyRecivingProgress(qint64,qint64)));
}

void DownloadMember::replyRecivingProgress(qint64 bytesRecived, qint64 bytesTotal)
{
    emit downloadSpeedChanged(speedCounter->bytesReciving(bytesRecived));
    emit downloadProgressChanged((100 * bytesRecived / bytesTotal));
}

void DownloadMember::replyRecivingFinished()
{
    if (reply->operation() == QNetworkAccessManager::HeadOperation)
    {
        prepareDownload();
    }
    else if (reply->operation() == QNetworkAccessManager::GetOperation)
    {
        saveDataAndContinue();
        if (parts.length() > 1)
        {
            startDownloading();
        }
        else
        {
            state = 4;
            parts.clear();
            emit downloadIsFinished();
        }
    }

}
