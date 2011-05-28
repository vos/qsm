/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <alexander.vos@stud.fh-swf.de>,
**                    Till Althaus <till.althaus@stud.fh-swf.de>
**
** This file is part of Qt Slideshow Manager (QSM).
**
** QSM is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** QSM is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with QSM.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "scanfolderthread.h"

#include <QMetaType>

const QStringList ScanFolderThread::FILE_FILTERS = QStringList() << "*.jpg" << "*.jpeg"
        << "*.png" << "*.bmp" << "*.tiff" << "*.gif" << "*.ico";

ScanFolderThread::ScanFolderThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
}

void ScanFolderThread::setFolder(const QString &path, QDir::SortFlags sort, bool recursive)
{
    m_path = path;
    m_sort = sort;
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
    QFileInfoList files = dir.entryInfoList(ScanFolderThread::FILE_FILTERS, QDir::Files | QDir::Readable, m_sort);
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
