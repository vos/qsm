#include "slideshowimage.h"

#include <QFileInfo>

SlideshowImage::SlideshowImage(const QString &path, int randomFactor, const QString &comment)
    : m_path(path), m_randomFactor(randomFactor), m_comment(comment)
{
    m_name = QFileInfo(path).fileName();
}

SlideshowImage::SlideshowImage(const SlideshowImage &image)
{
    m_path = image.m_path;
    m_name = image.m_name;
    m_randomFactor = image.m_randomFactor;
    m_comment = image.m_comment;
}

SlideshowImage& SlideshowImage::operator =(const SlideshowImage &image)
{
    if (this != &image) {
        m_path = image.m_path;
        m_name = image.m_name;
        m_randomFactor = image.m_randomFactor;
        m_comment = image.m_comment;
    }
    return *this;
}

bool SlideshowImage::operator ==(const SlideshowImage &image)
{
    return m_path == image.m_path;
}
