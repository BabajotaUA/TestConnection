#include "downloadmanager.h"
#include <QtCore/QDebug>
#include <QtCore/QStringList>

DownloadManager::DownloadManager()
{
}

DownloadManager::~DownloadManager()
{
}

QSharedPointer<DownloadMember> DownloadManager::downloadAt(const int& index)
{
    return downloadsList.at(index);
}

QSharedPointer<DownloadMember> DownloadManager::addNewDownload(const QString &sourceURL, const QString &destinationDirectory)
{
    auto source = QUrl(sourceURL);
    auto destination = destinationDirectory + source.path().split('/').last();
    auto newMember = QSharedPointer<DownloadMember>(new DownloadMember(source, destination));
    downloadsList.append(newMember);

    return newMember;
}

void DownloadManager::cancelDownload(DownloadMember *download)
{
}

void DownloadManager::pauseDownload(DownloadMember *download)
{
}

void DownloadManager::startDownload(DownloadMember *download)
{
    switch(download->getState())
    {
    case 0:
        qDebug() << "Download is preparing...";
        connect(download,SIGNAL(downloadIsReadyToStart()),download,SLOT(startDownloading()));
        break;
    case 1:
        download->startDownloading();
        break;
    default:
        qDebug() << "Download is at unknown state";
        break;
    }
}

void DownloadManager::startAllDownloads()
{
    for (int i=0;i<downloadsList.length();++i)
        startDownload(downloadsList.at(i).value);
}




