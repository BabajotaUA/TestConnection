#pragma once

#include "sender.h"
#include "filesaver.h"
#include "speedcounter.h"
#include <QtCore/QSharedPointer>
#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

class DownloadMember : public QObject
{
    Q_OBJECT
public:
    explicit DownloadMember(const QUrl& source, const QString& destination, QObject *parent = nullptr);
    virtual ~DownloadMember();

    enum State {NotReady, ReadyToStart, Downloading, Pause, Finished};
    int getState() const;

signals:
    void downloadIsReadyToStart();
    void downloadIsFinished();
    void downloadProgressChanged(qreal);
    void downloadSpeedChanged(qint64);
    
public slots:
    void startDownloading();
    void pauseDownloading();
    void cancelDownloading();

private slots:
    void replyReciving(QNetworkReply* newReply);
    void replyRecivingProgress(qint64 bytesRecived, qint64 bytesTotal);
    void replyRecivingError(QNetworkReply::NetworkError);
    void replyRecivingFinished();

private:
    void splitParts(const qint64 &minSplitSize, const qint64 &startByte = 0);
    void saveDataAndContinue();
    void chsngePartList();
    bool collectingHeaderData();
    void prepareDownload();
    QString fileDestination() const;

    QSharedPointer<Sender> sender;
    QSharedPointer<FileSaver> fileSaver;
    QSharedPointer<SpeedCounter> speedCounter;
    QSharedPointer<QNetworkReply> reply;

    QList<qint64> parts;
    QString fileName, destination;
    qint64 fileSize, PART_SIZE, bytesDownloaded, currentPartSize;
    State currentState;
};
