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

#include "imageloaderpool.h"

#include "imageloaderthread.h"

ImageLoaderPool::ImageLoaderPool(int maxThreadCount, QObject *parent) :
    QObject(parent)
{
    if (maxThreadCount <= 0)
        m_maxThreadCount = QThread::idealThreadCount();
}

ImageLoaderPool::~ImageLoaderPool()
{
    clear();
    qDeleteAll(m_activeThreads);
}

void ImageLoaderPool::start(ImageLoader *imageLoader)
{
    if (m_activeThreads.count() < m_maxThreadCount)
        addThread(imageLoader);
    else
        m_queue.enqueue(imageLoader);
}

void ImageLoaderPool::thread_finished()
{
    ImageLoaderThread *thread = qobject_cast<ImageLoaderThread*>(sender());
    if (!thread) return;

    m_activeThreads.remove(thread);
    thread->deleteLater();

    if (m_queue.count() > 0)
        addThread(m_queue.dequeue());
}

void ImageLoaderPool::clear()
{
    // remove all pending threads
    foreach (ImageLoader *loader, m_queue) {
        if (loader->autoDelete())
            delete loader;
    }
    m_queue.clear();

    // terminate all runing threads (not possible without memory leaks)
//    foreach (ImageLoaderThread *thread, m_activeThreads) {
//        thread->terminate();
//        thread->wait();
//        delete thread;
//    }
}

void ImageLoaderPool::addThread(ImageLoader *imageLoader)
{
    ImageLoaderThread *thread = new ImageLoaderThread(imageLoader, this);
    connect(thread, SIGNAL(finished()), SLOT(thread_finished()));
    m_activeThreads.insert(thread);
    thread->start(QThread::NormalPriority);
}
