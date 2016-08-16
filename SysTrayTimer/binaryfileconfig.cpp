#include "binaryfileconfig.h"

BinaryFileConfig::BinaryFileConfig(const QString filename)
{
    mFile = new QFile(filename);
}

BinaryFileConfig::~BinaryFileConfig()
{
    delete mFile;
}

void BinaryFileConfig::write_TimeToCount(qint32 timeToCount)
{
    this->timeToCount = timeToCount;

    updateAllVariables();
}

void BinaryFileConfig::write_RemainingTime(qint32 remTime)
{
    this->remainingTime = remTime;

    updateAllVariables();
}

void BinaryFileConfig::write_DesiredLanguage(quint32 language)
{
    this->desiredLanguage = language;

    updateAllVariables();
}

void BinaryFileConfig::write_ShortBreaks(bool shortBreak)
{
    this->shortBreakActive = shortBreak;

    updateAllVariables();
}

void BinaryFileConfig::write_LaunchBreak(bool launchBreak)
{
    this->launchBreakActive = launchBreak;

    updateAllVariables();
}

void BinaryFileConfig::write_DesiredIcon(quint32 comboIconIndex)
{
    this->desiredIcon = comboIconIndex;

    updateAllVariables();
}

quint32 BinaryFileConfig::readHeaderFile()
{
    readAllVariables();

    return this->HEADER_FILE;
}

qint32 BinaryFileConfig::read_TimeToCount()
{
    readAllVariables();

    return this->timeToCount;
}

qint32 BinaryFileConfig::read_RemainingTime()
{
    readAllVariables();

    return this->remainingTime;
}

quint32 BinaryFileConfig::read_DesiredLanguage()
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

void BinaryFileConfig::openFileForRead()
{
    if(!mFile->open(QIODevice::ReadOnly))
    {
        // something went wrong here
    }
}

void BinaryFileConfig::openFileForWrite()
{
    if(!mFile->open(QIODevice::WriteOnly))
    {
        // something went wrong here
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
    this->timeToCount = 0;
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

    mFile->close();
}

