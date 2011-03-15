#include "imageloaderthread.h"

ImageLoaderThread::ImageLoaderThread(ImageLoader *loader, QObject *parent) :
    QThread(parent), m_loader(loader)
{
    Q_ASSERT(loader);
}

ImageLoaderThread::~ImageLoaderThread()
 {
    if (m_loader->autoDelete())
        delete m_loader;
}

void ImageLoaderThread::run()
{
    m_loader->run();
}
