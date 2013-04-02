#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QFile>

class FileSaver : public QObject
{
    Q_OBJECT
public:
    explicit FileSaver(const QString& fileRepository, QObject *parent = nullptr);
    virtual ~FileSaver();

    void saveData(const QByteArray& data);
    QList<QString> getDownloadedParts();
    
signals:
    void filePrepared();
    
public slots:
    void prepareFile();
    void saveDownloadInfo(QByteArray &downloadedPart);

private:
    QFile file, infoFile;
    QString fileName;
    QByteArray downloadedData;
    
};
