#ifndef BINARYFILECONFIG_H
#define BINARYFILECONFIG_H

#include <QtCore>
#include <QFile>
#include <QDataStream>



class BinaryFileConfig
{
public:
    explicit BinaryFileConfig();
    ~BinaryFileConfig();

    // write to cfg. file
    void write_TimeToCount(qint32 timeToCount);
    void write_RemainingTime(qint32 remTime);
    void write_DesiredLanguage(quint32 language);
    void write_ShortBreaks(bool shortBreak=true);
    void write_LaunchBreak(bool launchBreak=true);
    void write_DesiredIcon(quint32 comboIconIndex);
    void write_DesiredGIF(quint32 comboGifIndex);

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
};

#endif // BINARYFILECONFIG_H
