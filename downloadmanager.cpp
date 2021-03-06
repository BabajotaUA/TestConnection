#include "downloadmanager.h"
#include <QtCore/QDebug>

DownloadManager::DownloadManager()
{
}

DownloadManager::~DownloadManager()
{
}

QSharedPointer<DownloadMember> DownloadManager::downloadAt(int index)
{
    return downloadsList.at(index);
}

QSharedPointer<DownloadMember> DownloadManager::addNewDownload(const QString &sourceURL, const QString &destinationDirectory)
{
    auto source = QUrl(sourceURL);
    auto newMember = QSharedPointer<DownloadMember>(new DownloadMember(source, destinationDirectory));
    downloadsList.append(newMember);

    return newMember;
}

int DownloadManager::count() const
{
    return downloadsList.length();
}

void DownloadManager::cancelDownload(DownloadMember *download)
{
    if (download->getState() == DownloadMember::Downloading || download->getState() == DownloadMember::Pause)
        download->cancelDownloading();
}

void DownloadManager::pauseDownload(DownloadMember *download)
{
    if (download->getState() == DownloadMember::Downloading)
        download->pauseDownloading();
}

void DownloadManager::startDownload(DownloadMember *download)
{
    switch(download->getState())
    {
    case DownloadMember::NotReady :
        qDebug() << "Download is preparing...";
        connect(download,SIGNAL(downloadIsReadyToStart()),download,SLOT(startDownloading()));
        break;
    case DownloadMember::ReadyToStart :
        download->startDownloading();
        break;
    case DownloadMember::Pause :
        download->startDownloading();
        break;
    default:
        qDebug() << "Download is at unknown state";
        break;
    }
}




