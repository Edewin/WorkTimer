#include "filemanipulator.h"


FileManipulator::FileManipulator(const QString &filename)
{
    mFile = new QFile(filename);
}

FileManipulator::~FileManipulator()
{
    delete mFile;
}

void FileManipulator::WriteToFile(const QString &dataToWrite)
{
    QTextStream writeStreamer(mFile);

    OpenFileForWrite();

    writeStreamer << dataToWrite;

    mFile->flush();
    mFile->close();
}

void FileManipulator::Append(const QString &dataToWrite)
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

    QString RxData;

    OpenFileForRead();

    if( mFile->size() > 10000 )
    {
        RxData = "file too big";
    }
    else
    {
        RxData = readStreamer.readAll();

    }

    mFile->close();

    return RxData;
}

void FileManipulator::ReplaceLastLineButKeepFirst2Elements()
{
    QTextStream readStreamer(mFile);

    QString RxData;

    OpenFileForReadWrite();

    qDebug() << mFile->size();

    // read Line by line all the Document and compare the dates
    while( !readStreamer.atEnd())
    {
        RxData = readStreamer.readLine();

        QStringList bufList = RxData.split(",");
        int comp = QString::compare(bufList.at(0),
                                    sysDate.currentDate().toString(),
                                    Qt::CaseInsensitive);

        if(comp == 0) // are the same
        {
            qDebug() << "dates are the same " + RxData;

            int lastLineInt = RxData.size();

            lastLineInt = mFile->size() - lastLineInt;
#ifdef __linux
            mFile->seek( lastLineInt - 1 );
#elif _WIN32
            mFile->seek( lastLineInt - 2 );
#endif
            // keep first 2 elements from lastLine and then update the line
            readStreamer << bufList.at(0) + "," + bufList.at(1) + ",";

            // delete all after -> bufList.at(1) + ","
            mFile->resize(lastLineInt);
            break;
        }
    }

    mFile->close();
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
        qWarning() << "Failed to open";
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

