#include "slideshowimage.h"

#include <QFileInfo>

SlideshowImage::SlideshowImage(const QString &path, int randomFactor, const QString &comment)
    : m_fileInfo(QFileInfo(path)), m_randomFactor(randomFactor), m_comment(comment)
{
}

SlideshowImage::SlideshowImage(const SlideshowImage &image)
{
    m_fileInfo = image.m_fileInfo;
    m_randomFactor = image.m_randomFactor;
    m_comment = image.m_comment;
}

SlideshowImage& SlideshowImage::operator =(const SlideshowImage &image)
{
    if (this != &image) {
        m_fileInfo = image.m_fileInfo;
        m_randomFactor = image.m_randomFactor;
        m_comment = image.m_comment;
    }
    return *this;
}

bool SlideshowImage::operator ==(const SlideshowImage &image)
{
    return m_fileInfo == image.m_fileInfo;
}
