#include "imageloaderpool.h"

#include "imageloaderthread.h"

ImageLoaderPool::ImageLoaderPool(QObject *parent) :
    QObject(parent)
{
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
