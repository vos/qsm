#include "slideshow.h"

Slideshow::Slideshow(const QString &path, Qsm::SortFlags sort, const QString &comment) :
    m_path(path), m_sort(sort), m_comment(comment)
{
}

Slideshow::Slideshow(const Slideshow &slideshow)
{
    m_path = slideshow.m_path;
    m_sort = slideshow.m_sort;
    m_comment = slideshow.m_comment;
    m_images = slideshow.m_images;
}

Slideshow& Slideshow::operator =(const Slideshow &slideshow)
{
    if (this != &slideshow) {
        m_path = slideshow.m_path;
        m_sort = slideshow.m_sort;
        m_comment = slideshow.m_comment;
        m_images = slideshow.m_images;
    }
    return *this;
}

void Slideshow::addImage(const SlideshowImage &image)
{
    // TODO mind sort order
    m_images.append(image);
}

void Slideshow::removeImage(int index)
{
    m_images.removeAt(index);
}

void Slideshow::removeImage(const SlideshowImage &image)
{
    m_images.removeOne(image);
}

void Slideshow::setSort(Qsm::SortFlags sort)
{
    m_sort = sort;
    // TODO reorder images
}
