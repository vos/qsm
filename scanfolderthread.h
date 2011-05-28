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

/*!
  \brief The ScanFolderThread class provides a simple interface to scan
         a folder (including all subfolders if desired) for images.
  \sa ImageLoader, ImageLoaderPool
 */
class ScanFolderThread : public QThread
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new ScanFolderThread to run the image scanning inside
             a separate thread.
      \param parent The parent object.
     */
    explicit ScanFolderThread(QObject *parent = 0);

    /*!
      \brief Sets the folder to be scanned to \a path.
      \param path The folder to be scanned.
      \param sort The sort order for the found images.
      \param recursive \c True if the folder should be scanned including all
             subfolders; otherwise \c false.
     */
    void setFolder(const QString &path, QDir::SortFlags sort = QDir::NoSort, bool recursive = false);

    /*!
      \brief Returns the current folder of this thread.
      \return The current folder.
     */
    inline const QString& folder() const { return m_path; }

    /*!
      \brief Returns the number of folders already scanned by this thread.
      \return The number of folders already scanned.
     */
    inline int count() const { return m_count; }

    /*!
      \brief Runs the folder scanning inside a separate thread.
      \sa start()
     */
    void run();

    /*!
      \brief Aborts the folder scanning \b after the current folder has been scanned.
     */
    void abort();

signals:

    /*!
      \brief This signal is emitted after a folder has been scanned.
      \param folder The folder which has been scanned.
      \param files The image files which has been found inside the folder (can be empty).
      \sa scanFolder()
     */
    void folderScanned(const QString &folder, const QFileInfoList &files);

private:

    static const QStringList FILE_FILTERS; //!< The image file filter.

    QString m_path;         //!< The path of the folder.
    QDir::SortFlags m_sort; //!< The sort flags to sort the found images.
    bool m_recursive;       //!< Indicates if the folder should be scanned recursive.
    volatile int m_count;   //!< The number of scanned folders.
    volatile bool m_abort;  //!< Indicates if the folder scan should be aborted.

    /*!
      \brief Helper method to scan a single folder for images.
      \param dir The folder to scan.
      \sa folderScanned()
     */
    void scanFolder(const QDir &dir);

    /*!
      \brief Helper method to scan a folder recursively for images.
      \param path The folder to scan recursively.
     */
    void scanFolderRecursively(const QString &path);

};

#endif // SCANFOLDERTHREAD_H
