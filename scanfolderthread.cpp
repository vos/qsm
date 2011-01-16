#include "scanfolderthread.h"

#include <QMetaType>

QStringList ScanFolderThread::FILE_FILTERS;

ScanFolderThread::ScanFolderThread(QObject *parent) :
    QThread(parent)
{
    if (ScanFolderThread::FILE_FILTERS.isEmpty())
        ScanFolderThread::FILE_FILTERS << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tiff" << "*.gif" << "*.ico";
}

void ScanFolderThread::setFolder(const QString &path, bool recursive)
{
    m_path = path;
    m_recursive = recursive;

    qRegisterMetaType<QFileInfoList>("QFileInfoList");
}

void ScanFolderThread::run()
{
    if (m_recursive)
        scanFolderRecursively(m_path);
    else
        scanFolder(QDir(m_path));
}

void ScanFolderThread::scanFolder(const QDir &dir)
{
    QFileInfoList files = dir.entryInfoList(ScanFolderThread::FILE_FILTERS, QDir::Files | QDir::Readable, QDir::Name);
    emit folderScanned(dir.path(), files);
}

void ScanFolderThread::scanFolderRecursively(const QString &path)
{
    QDir dir(path);
    scanFolder(dir);

    QStringList subfolders = dir.entryList(QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);
    foreach (const QString &folder, subfolders)
        scanFolderRecursively(dir.filePath(folder));
}
