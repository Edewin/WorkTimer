#include "filemanipulator.h"


FileManipulator::FileManipulator(const QString filename)
{
    mFile = new QFile(filename);
}

FileManipulator::~FileManipulator()
{
    delete mFile;
}



void FileManipulator::WriteToFile(const QString dataToWrite)
{

    QTextStream writeStreamer(mFile);

    OpenFileForWrite();

    writeStreamer << dataToWrite;

    mFile->flush();
    mFile->close();
}

void FileManipulator::Append(const QString dataToWrite)
{
    QTextStream appendStreamer(mFile);

    OpenFileForAppend();

    appendStreamer << dataToWrite;

    mFile->flush();
    mFile->close();
}

QString FileManipulator::ReadFromFile()
{
    QTextStream readStreamer(mFile);

    QString RxData = "";

    OpenFileForRead();

    RxData = readStreamer.readAll();

    mFile->close();

    return RxData;
}

void FileManipulator::OpenFileForRead()
{
    if(!mFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Couldn`t open the file";   // replace with messagebox
        return;
    }
}

void FileManipulator::OpenFileForWrite()
{
    if(!mFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Couldn`t open the file";   // replace with messagebox
        return;
    }
}

void FileManipulator::OpenFileForAppend()
{
    if(!mFile->open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Couldn`t open the file";   // replace with messagebox
        return;
    }
}

void FileManipulator::OpenFileForReadWrite()
{
    if(!mFile->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Couldn`t open the file";   // replace with messagebox
        return;
    }
}

