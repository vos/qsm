#include "slideshow.h"

Slideshow::Slideshow(const QString &name, Qsm::SortFlags sort, const QString &comment) :
    m_name(name), m_sort(sort), m_comment(comment)
{
}

Slideshow::Slideshow(const Slideshow &slideshow)
{
    m_name = slideshow.m_name;
    m_sort = slideshow.m_sort;
    m_comment = slideshow.m_comment;
    m_images = slideshow.m_images;
}

Slideshow& Slideshow::operator =(const Slideshow &slideshow)
{
    if (this != &slideshow) {
        m_name = slideshow.m_name;
        m_sort = slideshow.m_sort;
        m_comment = slideshow.m_comment;
        m_images = slideshow.m_images;
    }
    return *this;
}

void Slideshow::setSort(Qsm::SortFlags sort)
{
    if (sort == m_sort)
        return;

    m_sort = sort;
    // TODO reorder images
}

void Slideshow::addImage(const SlideshowImage &image)
{
    // TODO mind sort order
    m_images.append(image);
}

SlideshowImage* Slideshow::image(int index)
{
    if (index < 0 || index >= m_images.count())
        return NULL;

    return &m_images[index];
}

void Slideshow::removeImage(int index)
{
    if (index < 0 || index >= imageCount())
        return;

    m_images.removeAt(index);
}

void Slideshow::removeImage(const SlideshowImage &image)
{
    m_images.removeOne(image);
}
