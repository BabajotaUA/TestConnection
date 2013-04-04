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

    void savePart(const QByteArray& data, const QList<qint64> &parts);
    void prepareFile(const QList<qint64> &parts);
    void deleteInfoFile();
    
signals:
    void filePrepared();
    
public slots:


private:
    void prepareDestinationFile();
    void prepareInfoFile(const QList<qint64> &parts);

    QFile file, infoFile;
    QString fileName;
    QByteArray downloadedData;
    
};
