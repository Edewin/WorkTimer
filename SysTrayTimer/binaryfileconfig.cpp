#include "binaryfileconfig.h"

BinaryFileConfig::BinaryFileConfig(const QString &filename)
{
    mFile = new QFile(filename);

    initVariables();
}

BinaryFileConfig::~BinaryFileConfig()
{
    delete mFile;
}

void BinaryFileConfig::writeTimeToCount(qint32 timeToCount)
{
    this->timeToCount = timeToCount;

    updateAllVariables();
}

void BinaryFileConfig::writeRemainingTime(qint32 remTime)
{
    this->remainingTime = remTime;

    updateAllVariables();
}

void BinaryFileConfig::writeDesiredLanguage(quint32 language)
{
    this->desiredLanguage = language;

    updateAllVariables();
}

void BinaryFileConfig::writeShortBreaks(bool shortBreak)
{
    this->shortBreakActive = shortBreak;

    updateAllVariables();
}

void BinaryFileConfig::writeLaunchBreak(bool launchBreak)
{
    this->launchBreakActive = launchBreak;

    updateAllVariables();
}

void BinaryFileConfig::writeDesiredIcon(quint32 comboIconIndex)
{
    this->desiredIcon = comboIconIndex;

    updateAllVariables();
}

void BinaryFileConfig::writeDesiredGIF(quint32 comboGifIndex)
{
    this->desiredGif = comboGifIndex;

    updateAllVariables();
}

void BinaryFileConfig::writeCurrentDate(QString currentDate)
{
    this->dateString = currentDate;

    updateAllVariables();
}

qint64 BinaryFileConfig::getFileSize()
{
    return mFile->size();
}

quint32 BinaryFileConfig::readHeaderFile()
{
    readAllVariables();

    return this->HEADER_FILE;
}

qint32 BinaryFileConfig::readTimeToCount()
{
    readAllVariables();

    return this->timeToCount;
}

qint32 BinaryFileConfig::readRemainingTime()
{
    readAllVariables();

    return this->remainingTime;
}

quint32 BinaryFileConfig::readDesiredLanguage()
{
    readAllVariables();

    return this->desiredLanguage;
}

bool BinaryFileConfig::readShortBreak()
{
    readAllVariables();

    return this->shortBreakActive;
}

bool BinaryFileConfig::readLaunchBreak()
{
    readAllVariables();

    return this->launchBreakActive;
}

quint32 BinaryFileConfig::readDesiredIcon()
{
    readAllVariables();

    return this->desiredIcon;
}

quint32 BinaryFileConfig::readDesiredGif()
{
    readAllVariables();

    return this->desiredGif;
}

QString BinaryFileConfig::readCurrentDate()
{
    readAllVariables();

    return this->dateString;
}

void BinaryFileConfig::openFileForRead()
{
    if(!mFile->open(QIODevice::ReadOnly))
    {
        // something went wrong here
        qDebug() << "couldn`t open the file for read";
    }
}

void BinaryFileConfig::openFileForWrite()
{
    if(!mFile->open(QIODevice::WriteOnly))
    {
        // something went wrong here
        qDebug() << "couldn`t open the file for write";
    }
}

void BinaryFileConfig::initVariables()
{
    this->HEADER_FILE = 0xE0D0A016;

    this->desiredGif = 0;
    this->desiredIcon = 0;
    this->desiredLanguage = 0;

    this->launchBreakActive = true;
    this->shortBreakActive = true;

    this->remainingTime = 0;
    this->timeToCount = 31200;      // 8:40:00 (hh:MM:ss)
}

void BinaryFileConfig::updateAllVariables()
{
    openFileForWrite();
    QDataStream out(mFile);

    out << (quint32) this->HEADER_FILE;
    out << (qint32) this->timeToCount;
    out << (qint32) this->remainingTime;
    out << (quint32) this->desiredLanguage;
    out << (quint32) this->desiredIcon;
    out << (quint32) this->desiredGif;
    out << (bool) this->shortBreakActive;
    out << (bool) this->launchBreakActive;
    out << (QString) this->dateString;

    mFile->flush();
    mFile->close();
}

void BinaryFileConfig::readAllVariables()
{
    openFileForRead();
    QDataStream in(mFile);

    in >> this->HEADER_FILE;
    in >> this->timeToCount;
    in >> this->remainingTime;
    in >> this->desiredLanguage;
    in >> this->desiredIcon;
    in >> this->desiredGif;
    in >> this->shortBreakActive;
    in >> this->launchBreakActive;
    in >> this->dateString;

    mFile->close();
}

