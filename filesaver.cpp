#include "filesaver.h"

FileSaver::FileSaver(const QString &fileRepository, QObject *parent) :
    QObject(parent)
{
    file.setFileName(fileRepository);
    infoFile.setFileName(fileRepository + ".inf");
}

FileSaver::~FileSaver()
{
}

void FileSaver::saveData(const QByteArray &data)
{
    if (!file.open(QIODevice::WriteOnly))
        return;

    file.write(data);

    file.flush();
    file.close();
}

QList<QString> FileSaver::getDownloadedParts()
{
    if (!infoFile.open(QIODevice::ReadOnly))
        return QList<QString>();

    QList<QString> result;
    QList<QByteArray> rawList = infoFile.readAll().split('*');
    foreach (QByteArray rawItem, rawList)
    {
        result.append(QString(rawItem));
    }

    return result;
}

void FileSaver::prepareFile()
{
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
        return;

    file.flush();
    file.close();
    emit filePrepared();
}

void FileSaver::saveDownloadInfo(QByteArray &downloadedPart)
{
    if (!infoFile.open(QIODevice::WriteOnly | QIODevice::Append))
        return;

    infoFile.write(downloadedPart.append('*'));

    infoFile.flush();
    infoFile.close();
}
