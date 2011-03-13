#include "imageloaderpool.h"

ImageLoaderPool::ImageLoaderPool(QObject *parent) :
    QThreadPool(parent)
{
}

void ImageLoaderPool::clear()
{
    // TODO terminate all threads
    //d_func()->reset();
}
