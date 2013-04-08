#include "downloadmember.h"
#include <QDebug>
#include <QStringList>
#include <stdexcept>

DownloadMember::DownloadMember(const QUrl &source, const QString &destination, QObject *parent) :
    QObject(parent)
{
    fileName = source.path().split('/').last();
    fileDestination = destination;
    partSize = 2000000;
    currentbytesDownloaded = 0;
    bytesDownloaded = 0;
    currentPartSize = 0;
    maxFlows = 5;
    fileSize = 0;
    currentState = NotReady;

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

int DownloadMember::getState() const
{
    return currentState;
}

void DownloadMember::startDownloading()
{
    if (parts.length()>1)
        sender->download("bytes=" + QByteArray::number(parts[0]) + "-" + QByteArray::number(parts[1]-1));
    else
        sender->download("");

    speedCounter->start();
    currentState = Downloading;
}

void DownloadMember::pauseDownloading()
{
    currentState = Pause;
    parts[0] += currentbytesDownloaded;
    fileSaver->savePart(reply->readAll(), parts);
    reply->abort();
}

void DownloadMember::cancelDownloading()
{
    currentState = NotReady;
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
    fileSize = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    if (fileSize == 0)
    {
        throw std::runtime_error("File header reading ERROR");
    }
    splitParts(partSize);
    fileSaver->prepareFile(parts);
    currentState = ReadyToStart;
    emit downloadIsReadyToStart();
}

void DownloadMember::saveDataAndContinue()
{
    speedCounter->stop();
    chsngePartList();
    bytesDownloaded += currentPartSize;
    fileSaver->savePart(reply->readAll(), parts);
}

void DownloadMember::chsngePartList()
{
    if (reply->rawHeader("Content-Range").isEmpty())
        parts.clear();
    else
        parts.removeFirst();
}

void DownloadMember::replyReciving(QNetworkReply *newReply)
{
    reply = QSharedPointer<QNetworkReply>( newReply );
    connect(reply.data(),SIGNAL(error(QNetworkReply::NetworkError)),SLOT(replyRecivingError(QNetworkReply::NetworkError)));
    connect(reply.data(),SIGNAL(finished()),SLOT(replyRecivingFinished()));

    if (reply->operation() == QNetworkAccessManager::GetOperation)
    {
        connect(reply.data(),SIGNAL(downloadProgress(qint64,qint64)),SLOT(replyRecivingProgress(qint64,qint64)));
    }
}

void DownloadMember::replyRecivingProgress(qint64 bytesRecived, qint64 bytesTotal)
{
    currentPartSize = bytesTotal;
    currentbytesDownloaded = bytesRecived;
    emit downloadSpeedChanged(speedCounter->bytesReciving(bytesRecived));
    emit downloadProgressChanged((100 * (bytesDownloaded + bytesRecived) / fileSize));

    qDebug() << fileName << (100 * (bytesDownloaded + bytesRecived) / fileSize);
}

void DownloadMember::replyRecivingError(QNetworkReply::NetworkError)
{
    qDebug() << reply->errorString();
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
            currentState = Finished;
            parts.clear();
            fileSaver->deleteInfoFile();
            emit downloadIsFinished();
        }
    }

}
