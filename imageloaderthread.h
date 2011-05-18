#ifndef IMAGELOADERTHREAD_H
#define IMAGELOADERTHREAD_H

#include <QThread>

class ImageLoader;

class ImageLoaderThread : public QThread
{
    Q_OBJECT

public:
    explicit ImageLoaderThread(ImageLoader *loader, QObject *parent = 0);
    ~ImageLoaderThread();

    inline ImageLoader* loader() const { return m_loader; }

private:
    ImageLoader *m_loader;

    void run();
};

#endif // IMAGELOADERTHREAD_H
