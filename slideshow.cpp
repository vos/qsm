#include "slideshow.h"

#include <QDir>

const QString Slideshow::FILE_EXTENSION = ".xml";

Slideshow::Slideshow(const QString &name, Qsm::SortFlags sort, const QString &comment) :
    m_name(name), m_sort(sort), m_comment(comment), m_changed(false)
{
}

Slideshow::Slideshow(const Slideshow &slideshow)
{
    m_name = slideshow.m_name;
    m_sort = slideshow.m_sort;
    m_comment = slideshow.m_comment;
    m_images = slideshow.m_images;
    m_changed = slideshow.m_changed;
}

Slideshow& Slideshow::operator =(const Slideshow &slideshow)
{
    if (this != &slideshow) {
        m_name = slideshow.m_name;
        m_sort = slideshow.m_sort;
        m_comment = slideshow.m_comment;
        m_images = slideshow.m_images;
        m_changed = slideshow.m_changed;
    }
    return *this;
}

void Slideshow::setName(const QString &name)
{
    if (name == m_name)
        return;

    m_name = name;
    m_changed = true;
}

QString Slideshow::path(const QString &dir) const
{
    return dir + QDir::separator() + m_name + Slideshow::FILE_EXTENSION;
}

void Slideshow::setSort(Qsm::SortFlags sort)
{
    if (sort == m_sort)
        return;

    m_sort = sort;
    // TODO reorder images

    m_changed = true;
}

void Slideshow::setComment(const QString &comment)
{
    if (comment == m_comment)
        return;

    m_comment = comment;
    m_changed = true;
}

void Slideshow::addImage(const SlideshowImage &image)
{
    // TODO mind sort order
    m_images.append(image);
    m_changed = true;
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
    m_changed = true;
}

void Slideshow::removeImage(const SlideshowImage &image)
{
    if (m_images.removeOne(image))
        m_changed = true;
}

void Slideshow::clearImages()
{
    if (m_images.count() > 0) {
        m_images.clear();
        m_changed = true;
    }
}
