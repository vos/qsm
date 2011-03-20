#include "slideshowimage.h"

SlideshowImage::SlideshowImage(const QString &path, int loadFactor, const QString &comment)
    : m_path(path), m_loadFactor(loadFactor), m_comment(comment)
{
}

SlideshowImage::SlideshowImage(const SlideshowImage &image)
{
    m_path = image.m_path;
    m_loadFactor = image.m_loadFactor;
    m_comment = image.m_comment;
}

SlideshowImage& SlideshowImage::operator =(const SlideshowImage &image)
{
    if (this != &image) {
        m_path = image.m_path;
        m_loadFactor = image.m_loadFactor;
        m_comment = image.m_comment;
    }
    return *this;
}

bool SlideshowImage::operator ==(const SlideshowImage &image)
{
    return m_path == image.m_path;
}
