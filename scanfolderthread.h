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

#ifndef SCANFOLDERTHREAD_H
#define SCANFOLDERTHREAD_H

#include <QThread>
#include <QFileInfoList>
#include <QDir>
#include <QStringList>

class ScanFolderThread : public QThread
{
    Q_OBJECT

public:
    explicit ScanFolderThread(QObject *parent = 0);
    void setFolder(const QString &path, QDir::SortFlags sort = QDir::NoSort, bool recursive = false);
    inline const QString& getFolder() const { return m_path; }
    inline int count() const { return m_count; }
    void run();
    void abort();

signals:
    void folderScanned(const QString &folder, const QFileInfoList &files);

private:
    static QStringList FILE_FILTERS; // TODO should be const

    QString m_path;
    QDir::SortFlags m_sort;
    bool m_recursive;
    volatile int m_count;
    volatile bool m_abort;

    void scanFolder(const QDir &dir);
    void scanFolderRecursively(const QString &path);
};

#endif // SCANFOLDERTHREAD_H
