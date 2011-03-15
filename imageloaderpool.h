#ifndef IMAGELOADERPOOL_H
#define IMAGELOADERPOOL_H

#include <QObject>
#include <QSet>
#include <QQueue>

#include "imageloader.h"

class ImageLoaderThread;

class ImageLoaderPool : public QObject
{
    Q_OBJECT

public:
    explicit ImageLoaderPool(QObject *parent = 0);
    ~ImageLoaderPool();

    void start(ImageLoader *imageLoader);
    inline int maxThreadCount() const { return m_maxThreadCount; }
    inline int activeThreadCount() const { return m_activeThreads.count(); }

public slots:
    void clear();

private slots:
    void thread_finished();

private:
    int m_maxThreadCount;
    QSet<ImageLoaderThread*> m_activeThreads;
    QQueue<ImageLoader*> m_queue;

    void addThread(ImageLoader *imageLoader);
};

#endif // IMAGELOADERPOOL_H
