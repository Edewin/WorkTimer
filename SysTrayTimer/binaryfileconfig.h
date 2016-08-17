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
    void write_TimeToCount(const qint32 timeToCount);
    void write_RemainingTime(const qint32 remTime);
    void write_DesiredLanguage(const quint32 language);
    void write_ShortBreaks(const bool shortBreak=true);
    void write_LaunchBreak(const bool launchBreak=true);
    void write_DesiredIcon(const quint32 comboIconIndex);
    void write_DesiredGIF(const quint32 comboGifIndex);
    void write_CurrentDate(const QString currentDate);

    void initVariables();

    quint32 readHeaderFile();
    qint32 read_TimeToCount();
    qint32 read_RemainingTime();
    quint32 read_DesiredLanguage();
    bool readShortBreak();
    bool readLaunchBreak();
    quint32 readDesiredIcon();
    quint32 readDesiredGif();
    QString readCurrentDate();

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
    QString dateString;

    // file
    QFile *mFile;
};

#endif // BINARYFILECONFIG_H
