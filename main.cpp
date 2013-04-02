#include "downloadmanager.h"
#include <QCoreApplication>
#include <QDebug>

DownloadManager downloadManager;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    downloadManager.addNewDownload("http://freedownloads.last.fm/download/436344583/If%2BYou%2BWanna.mp3","E:/");
    downloadManager.addNewDownload("http://stream.get-tune.net/file/84506369/17645845/3257725966/298c3e49b56edbb5/Nirvana_-_In_Bloom_(get-tune.net).mp3","E:/");
    downloadManager.startDownload(downloadManager.downloadAt(0).value);
    downloadManager.startDownload(downloadManager.downloadAt(1).value);
    downloadManager.startDownload(downloadManager.addNewDownload("http://cs416327.vk.me/v416327355/492f/zYtqx5wz9qA.jpg","E:/").value);
    
    return a.exec();
}
