#ifndef FILEMANIPULATOR_H
#define FILEMANIPULATOR_H

#include <QFile>
#include <QString>
#include <QIODevice>
#include <QDir>
#include <QTextStream>
#include <QDebug>


class FileManipulator
{
public:
    explicit FileManipulator(const QString filename);

    ~FileManipulator();

    void WriteToFile(const QString dataToWrite);
    void Append(const QString dataToWrite);
    QString ReadFromFile();

protected:
    void OpenFileForRead();
    void OpenFileForWrite();
    void OpenFileForAppend();
    void OpenFileForReadWrite();

private:
    QFile *mFile;

signals:

public slots:
};

#endif // FILEMANIPULATOR_H
