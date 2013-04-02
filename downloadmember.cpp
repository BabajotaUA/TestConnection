#include "downloadmember.h"
#include <QDebug>
#include <QStringList>

DownloadMember::DownloadMember(const QUrl &source, const QString &destination, QObject *parent) :
    QObject(parent)
{
    fileName = source.path().split('/').last();
    fileDestination = destination;
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
    sender->startDownloading();
    speedCounter->start();
    state = 2;
}

void DownloadMember::resumeDownloading()
{
}

void DownloadMember::pauseDownloading()
{
    state = 3;
}

void DownloadMember::cancelDownloading()
{
}


void DownloadMember::splitParts(const qint64 &minSplitSize, const qint64 &startByte = 0)
{
    auto partEnd = startByte + minSplitSize;
    auto partStart = startByte;

    for (; partEnd < fileSize; partStart += minSplitSize)
    {
        parts.append("bytes=" + QString::number(partStart) + "-" + QString::number(partEnd));
        qDebug() << fileName << parts.last();
        partEnd += minSplitSize;
    }
    parts.append("bytes=" + QString::number(partStart) + "-" + QString::number(fileSize));
    qDebug() << fileName << parts.last();
}

void DownloadMember::prepareDownload()
{
    fileSize = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    splitParts(2000000);
    state = 1;
    emit downloadIsReadyToStart();
}

void DownloadMember::replyReciving(QNetworkReply *newReply)
{
    reply = QSharedPointer<QNetworkReply>( newReply );

    connect(reply.data(),SIGNAL(finished()),SLOT(replyRecivingFinished()));

    if (reply->operation() == QNetworkAccessManager::GetOperation)
    {
        connect(reply.data(),SIGNAL(downloadProgress(qint64,qint64)),SLOT(replyRecivingProgress(qint64,qint64)));
    }
}

void DownloadMember::replyRecivingProgress(qint64 bytesRecived, qint64 bytesTotal)
{
    qDebug() << fileName << speedCounter->bytesReciving(bytesRecived, bytesTotal)/1024 << "KB/s";
}

void DownloadMember::replyRecivingFinished()
{
    if (reply->operation() == QNetworkAccessManager::HeadOperation)
    {
        prepareDownload();
    }
}
