#include "imageloader.h"

ImageLoader::ImageLoader(const QString &imagePath, int index, QObject *parent) :
    QObject(parent), m_imagePath(imagePath), m_index(index), m_scaleWidth(0), m_scaleHeight(0)
{
}

void ImageLoader::setScaleSize(int width, int height)
{
    m_scaleWidth = width;
    m_scaleHeight = height;
}

void ImageLoader::run()
{
    QImage image(m_imagePath);
    if (m_scaleWidth > 0 && m_scaleHeight > 0) {
        emit imageLoaded(image.scaled(m_scaleWidth, m_scaleHeight, Qt::KeepAspectRatio,
                                      Qt::FastTransformation), image.width(), image.height(), m_index);
    }
    else {
        emit imageLoaded(image, image.width(), image.height(), m_index);
    }
}
