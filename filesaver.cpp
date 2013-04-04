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

void FileSaver::savePart(const QByteArray &data)
{
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
        return;

    file.write(data);

    file.flush();
    file.close();
}

void FileSaver::prepareFile(const QList<qint64> &parts)
{
    prepareDestinationFile();
    prepareInfoFile(parts);
    emit filePrepared();
}

void FileSaver::prepareDestinationFile()
{
    if (!file.open(QIODevice::WriteOnly))
        return;

    file.flush();
    file.close();
}

void FileSaver::prepareInfoFile(const QList<qint64> &parts)
{
    if (!infoFile.open(QIODevice::WriteOnly))
        return;

    foreach (qint64 part, parts)
        infoFile.write(QByteArray::number(part) + ",");

    infoFile.flush();
    infoFile.close();
}
