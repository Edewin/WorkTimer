#ifndef FILEMANIPULATOR_H
#define FILEMANIPULATOR_H

#include <QFile>
#include <QString>
#include <QIODevice>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QDate>


class FileManipulator
{
public:
    explicit FileManipulator(const QString &filename);

    ~FileManipulator();

    void WriteToFile(const QString &dataToWrite);
    void Append(const QString &dataToWrite);
    QString ReadFromFile();
    void ReplaceLastLineButKeepFirst2Elements();

protected:
    void OpenFileForRead();
    void OpenFileForWrite();
    void OpenFileForAppend();
    void OpenFileForReadWrite();

private:
    QFile *mFile;

    QDate sysDate;

signals:

public slots:
};

#endif // FILEMANIPULATOR_H
