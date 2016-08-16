#ifndef BINARYFILECONFIG_H
#define BINARYFILECONFIG_H

#include <QtCore>
#include <QFile>
#include <QDir>
#include <QDataStream>

class BinaryFileConfig
{
public:
    explicit BinaryFileConfig(const QString filename);
    ~BinaryFileConfig();

    // write to cfg. file
    void write_TimeToCount(qint32 timeToCount);
    void write_RemainingTime(qint32 remTime);
    void write_DesiredLanguage(quint32 language);
    void write_ShortBreaks(bool shortBreak=true);
    void write_LaunchBreak(bool launchBreak=true);
    void write_DesiredIcon(quint32 comboIconIndex);
    void write_DesiredGIF(quint32 comboGifIndex);

    void initVariables();

    quint32 readHeaderFile();
    qint32 read_TimeToCount();
    qint32 read_RemainingTime();
    quint32 read_DesiredLanguage();
    bool readShortBreak();
    bool readLaunchBreak();
    quint32 readDesiredIcon();
    quint32 readDesiredGif();

private:
    void openFileForRead();
    void openFileForWrite();

    void updateAllVariables();
    void readAllVariables();


    // variables
    quint32 HEADER_FILE;
    qint32 timeToCount, remainingTime;
    quint32 desiredLanguage, desiredIcon, desiredGif;
    bool shortBreakActive, launchBreakActive;


    // file
    QFile *mFile;
};

#endif // BINARYFILECONFIG_H
