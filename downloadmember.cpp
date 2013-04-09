#include "downloadmember.h"
#include <QDebug>
#include <QStringList>
#include <stdexcept>

DownloadMember::DownloadMember(const QUrl &source, const QString &destination, QObject *parent) :
    QObject(parent)
{
    fileName = source.path().split('/').last();
    this->destination = destination;
    PART_SIZE = 2000000;
    bytesDownloaded = 0;
    currentPartSize = 0;
    fileSize = 0;
    currentState = NotReady;

    sender = QSharedPointer<Sender>(new Sender(source,this));
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
    speedCounter->stop();
    auto save = reply->readAll();
    parts[0] += save.size();
    bytesDownloaded += save.size();
    fileSaver->savePart(save, parts);
    reply.clear();
    currentState = Pause;
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
    if (!collectingHeaderData())
    {
        qDebug() << "ERROR - reciving file headers";
        return;
    }
    fileSaver = QSharedPointer<FileSaver>(new FileSaver(fileDestination(),this));
    fileSaver->prepareFile(parts);
    currentState = ReadyToStart;
    emit downloadIsReadyToStart();
}

QString DownloadMember::fileDestination() const
{
    return destination + fileName;
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

bool DownloadMember::collectingHeaderData()
{
    qDebug() << "\n\n" << fileName
             << '\n' << reply->rawHeaderList()
             << "\n\n";

    if (reply->rawHeaderList().contains("Content-Length")) {
        fileSize = reply->rawHeader("Content-Length").toLongLong();
    }
    else {
        //throw "Content-Length is missing";
        return false;
    }

    if (reply->rawHeaderList().contains("Content-Disposition")) {
        auto headValue = QString(reply->rawHeader("Content-Disposition"));
        auto substringBegin = headValue.indexOf("filename=");
        if (substringBegin)
            fileName = headValue.mid(substringBegin + 10).split('"')[0];
    }

    if (reply->rawHeaderList().contains("Accept-Ranges")) {
        splitParts(PART_SIZE);
    }
    return true;
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
    emit downloadSpeedChanged(speedCounter->bytesReciving(bytesDownloaded + bytesRecived));
    emit downloadProgressChanged((100 * (bytesDownloaded + bytesRecived) / fileSize));
    //pauseDownloading();

    qDebug() << fileName << (100 * (bytesDownloaded + bytesRecived) / fileSize);
}

void DownloadMember::replyRecivingError(QNetworkReply::NetworkError)
{
    qDebug() << reply->errorString();
    reply.clear();
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
