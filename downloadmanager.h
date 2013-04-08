#pragma once

#include "downloadmember.h"
#include <QtCore/QSharedPointer>
#include <QtCore/QObject>
#include <QtCore/QList>

class DownloadManager : QObject
{
    Q_OBJECT
public:
    explicit DownloadManager();
    virtual ~DownloadManager();

    QSharedPointer<DownloadMember> downloadAt(int index);
    QSharedPointer<DownloadMember> addNewDownload(const QString& sourceURL,const QString& destinationDirectory);
    int count() const;

public slots:
    void cancelDownload(DownloadMember *download);
    void pauseDownload(DownloadMember *download);
    void startDownload(DownloadMember *download);
    void startAllDownloads();

private:
    QList< QSharedPointer<DownloadMember> > downloadsList;
    
};

