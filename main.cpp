#include "downloadmanager.h"
#include <QCoreApplication>
#include <QDebug>

DownloadManager downloadManager;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    downloadManager.addNewDownload("http://stream.get-tune.net/file/84506369/17645845/3257725966/298c3e49b56edbb5/Nirvana_-_In_Bloom_(get-tune.net).mp3","E:/");
    downloadManager.addNewDownload("http://r17---sn-5hn7snel.c.youtube.com/videoplayback?ratebypass=yes&ipbits=8&sparams=cp%2Cid%2Cip%2Cipbits%2Citag%2Cratebypass%2Csource%2Cupn%2Cexpire&newshard=yes&fexp=901448%2C932000%2C932004%2C906383%2C902000%2C901208%2C919512%2C929903%2C925714%2C931202%2C900821%2C900823%2C931203%2C931401%2C906090%2C909419%2C908529%2C930807%2C919373%2C930803%2C906836%2C920201%2C929602%2C930101%2C926403%2C900824%2C910223&expire=1365533546&key=yt1&ip=194.110.219.32&mt=1365509064&sver=3&cp=U0hVSlVOUV9JUENONV9NSFVGOldxYTUyQUY5SHRD&source=youtube&id=2b50e7631777893c&upn=05_U661I2dg&mv=m&ms=au&itag=22&signature=1FAA16502CD0A5758B2D9BC81210D570058CD8A1.B1256CB3D261128FDBF969AE01C8EC0BE6ACF74D&title=Blender+Basics+-+03+-+Modeling","E:/");
    downloadManager.startDownload(downloadManager.downloadAt(0).value);
    downloadManager.startDownload(downloadManager.downloadAt(1).value);
    downloadManager.startDownload(downloadManager.addNewDownload("http://cs416327.vk.me/v416327355/492f/zYtqx5wz9qA.jpg","E:/").value);


    
    return a.exec();
}
