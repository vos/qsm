#include "scanfolderthread.h"

#include <QMetaType>

QStringList ScanFolderThread::FILE_FILTERS;

ScanFolderThread::ScanFolderThread(QObject *parent) :
    QThread(parent)
{
    if (ScanFolderThread::FILE_FILTERS.isEmpty())
        ScanFolderThread::FILE_FILTERS << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tiff" << "*.gif" << "*.ico";

    qRegisterMetaType<QFileInfoList>("QFileInfoList");
}

void ScanFolderThread::setFolder(const QString &path, bool recursive)
{
    m_path = path;
    m_recursive = recursive;
}

void ScanFolderThread::run()
{
    m_count = 0;
    m_abort = false;

    if (m_recursive)
        scanFolderRecursively(m_path);
    else
        scanFolder(QDir(m_path));
}

void ScanFolderThread::scanFolder(const QDir &dir)
{
    QFileInfoList files = dir.entryInfoList(ScanFolderThread::FILE_FILTERS, QDir::Files | QDir::Readable, QDir::Name);
    m_count++;
    emit folderScanned(dir.path(), files);
}

void ScanFolderThread::scanFolderRecursively(const QString &path)
{
    if (m_abort)
        return;

    QDir dir(path);
    scanFolder(dir);

    QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);
    foreach (const QString &folder, subfolders)
        scanFolderRecursively(dir.filePath(folder));
}

void ScanFolderThread::abort()
{
    m_abort = true;
}
