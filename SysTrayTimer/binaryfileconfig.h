#ifndef BINARYFILECONFIG_H
#define BINARYFILECONFIG_H

#include <QtCore>
#include <QFile>
#include <QDir>
#include <QDataStream>

class BinaryFileConfig
{
public:
    BinaryFileConfig(const QString filename);
    ~BinaryFileConfig();

    // write to cfg. file
    void writeTimeToCount(qint32 timeToCount);
    void writeRemainingTime(qint32 remTime);
    void writeDesiredLanguage(quint32 language);
    void writeShortBreaks(bool shortBreak=true);
    void writeLaunchBreak(bool launchBreak=true);
    void writeDesiredIcon(quint32 comboIconIndex);
    void writeDesiredGIF(quint32 comboGifIndex);
    void writeCurrentDate(QString currentDate);


    quint32 readHeaderFile();
    qint32 readTimeToCount();
    qint32 readRemainingTime();
    quint32 readDesiredLanguage();
    bool readShortBreak();
    bool readLaunchBreak();
    quint32 readDesiredIcon();
    quint32 readDesiredGif();
    QString readCurrentDate();

private:
    void initVariables();

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
