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

#ifndef IMAGELOADERPOOL_H
#define IMAGELOADERPOOL_H

#include <QObject>
#include <QSet>
#include <QQueue>

#include "imageloader.h"

class ImageLoaderThread;

/*!
  \brief The ImageLoaderPool class manages a collection of ImageLoaderThreads.

  ImageLoaderPool manages and recyles individual ImageLoaderThread objects to
  help reduce thread creation costs.

  \sa ImageLoader, ImageLoaderThread
 */
class ImageLoaderPool : public QObject
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new ImageLoaderPool.
      \param maxThreadCount The maximum number of parallel running threads.
      \param parent The parent object.
      \note If \a maxThreadCount is less than or equal to \c 0, the ideal
            thread count will be used which is usually the number of logical
            processor cores.
      \sa maxThreadCount()
     */
    explicit ImageLoaderPool(int maxThreadCount = 0, QObject *parent = 0);

    /*!
      \brief Destroys the ImageLoaderPool. This method will delete all pending
             and active threads.
     */
    ~ImageLoaderPool();

    /*!
      \brief Adds a ImageLoader to this thread pool.

      Adds a ImageLoader to this thread pool. If the active thread count is
      less than the maximum thread count, the image loader will be started
      immediately; otherwise the image loader will ne enqueued to the waiting
      queue and the execution will be delayed until the other threads are
      processed.

      \param imageLoader The image loader to add.
      \note If ImageLoader::autoDelete() returns \c true, the ImageLoader
            will be deleted after the execution of ImageLoader::run().
      \sa maxThreadCount(), activeThreadCount(), clear()
     */
    void start(ImageLoader *imageLoader);

    /*!
      \brief Returns The maximum number of parallel running threads.
      \return The maximum number of parallel running threads.
      */
    inline int maxThreadCount() const { return m_maxThreadCount; }

    /*!
      \brief Returns The number of currently running threads.
      \return The number of currently running threads.
      */
    inline int activeThreadCount() const { return m_activeThreads.count(); }

public slots:

    /*!
      \brief Removes all pending threads from the waiting queue.
      \note The termination of currently active threads would lead to
            memory leaks or even a dead lock.
      \sa activeThreadCount()
     */
    void clear();

private slots:

    /*!
      \brief Schedules the finished thread for later deletion and starts a
             waiting one from the waiting queue (if any).
      \sa start()
     */
    void thread_finished();

private:

    int m_maxThreadCount;                     //!< The maximum number of parallel running threads.
    QSet<ImageLoaderThread*> m_activeThreads; //!< The set of currently running threads.
    QQueue<ImageLoader*> m_queue;             //!< The waiting queue.

    /*!
      \brief Adds a thread to the set of active running threads and starts it.
      \param imageLoader The image loader which will be executed inside a
             separate ImageLoaderThread.
     */
    void addThread(ImageLoader *imageLoader);

};

#endif // IMAGELOADERPOOL_H
