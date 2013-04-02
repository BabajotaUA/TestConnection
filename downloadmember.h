#pragma once

#include "sender.h"
#include "reciver.h"
#include "filesaver.h"
#include "speedcounter.h"
#include <QtCore/QSharedPointer>
#include <QtCore/QObject>

class DownloadMember : public QObject
{
    Q_OBJECT
public:
    explicit DownloadMember(const QUrl& source, const QString& destination, QObject *parent = nullptr);
    virtual ~DownloadMember();

    qint8 getState() const;

signals:
    void downloadIsReadyToStart();
    void downloadProgressChanged(qreal);
    void downloadSpeedChanged(qint64);
    
public slots:
    void startDownloading();
    void resumeDownloading();
    void pauseDownloading();
    void cancelDownloading();

private slots:
    void replyReciving(QNetworkReply* newReply);
    void replyRecivingProgress(qint64 bytesRecived, qint64 bytesTotal);
    void replyRecivingFinished();

private:
    void splitParts(const qint64 &minSplitSize, const qint64 &startByte);
    void prepareDownload();

    QSharedPointer<Sender> sender;
    QSharedPointer<FileSaver> fileSaver;
    QSharedPointer<SpeedCounter> speedCounter;
    QSharedPointer<QNetworkReply> reply;

    QList<QString> parts;
    QString fileName;
    QString fileDestination;
    qint64 fileSize;
    qint8 state, maxFlows;
};