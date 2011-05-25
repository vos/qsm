#include "slideshow.h"

#include <QDir>
#include <QDateTime>

const QString Slideshow::FILE_EXTENSION = ".xml";

Slideshow::Slideshow(const QString &name, Qsm::SortFlags sortFlags, const QString &comment) :
    m_name(name), m_comment(comment), m_changed(false)
{
    setSortFlags(sortFlags);
}

Slideshow::Slideshow(const Slideshow &slideshow)
{
    m_name = slideshow.m_name;
    m_sort = slideshow.m_sort;
    m_comparator = slideshow.m_comparator;
    m_comment = slideshow.m_comment;
    m_images = slideshow.m_images;
    m_changed = slideshow.m_changed;
}

Slideshow& Slideshow::operator =(const Slideshow &slideshow)
{
    if (this != &slideshow) {
        m_name = slideshow.m_name;
        m_sort = slideshow.m_sort;
        m_comparator = slideshow.m_comparator;
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

bool Slideshow::setSortFlags(Qsm::SortFlags sortFlags)
{
    if (sortFlags == m_sort)
        return false;

    m_sort = sortFlags;
    switch (m_sort) {
    case Qsm::Name: m_comparator = &Slideshow::sortNameAsc; break;
    case Qsm::NameReversed: m_comparator = &Slideshow::sortNameDesc; break;
    case Qsm::Date: m_comparator = &Slideshow::sortDateAsc; break;
    case Qsm::DateReversed: m_comparator = &Slideshow::sortDateDesc; break;
    default: m_comparator = NULL;
    }
    sort();

    m_changed = true;
    return true;
}

void Slideshow::setComment(const QString &comment)
{
    if (comment == m_comment)
        return;

    m_comment = comment;
    m_changed = true;
}

int Slideshow::addImage(const SlideshowImage &image)
{
    if (m_comparator && !(m_sort & Qsm::Unsorted)) {
        for (int i = 0; i < m_images.count(); ++i)
            if ((*m_comparator)(image, m_images.at(i))) {
                m_images.insert(i, image);
                m_changed = true;
                return i;
            }
    }
    // unsorted, image list empty or insert at end
    m_images.append(image);
    m_changed = true;
    return m_images.count() - 1;
}

SlideshowImage* Slideshow::image(int index)
{
    if (index < 0 || index >= m_images.count())
        return NULL;

    return &m_images[index];
}

void Slideshow::moveImage(int index, int delta)
{
    if (index < 0 || index >= m_images.count())
        return;
    int to = qBound(0, index + delta, m_images.count() - 1);
    if (index == to) return;
    if (!(m_sort & Qsm::Unsorted)) {
        m_sort = Qsm::Unsorted;
        m_comparator = NULL;
    }
    m_images.move(index, to);
    m_changed = true;
}

void Slideshow::removeImage(int index)
{
    if (index < 0 || index >= imageCount())
        return;

    m_images.removeAt(index);
    m_changed = true;
}

void Slideshow::clearImages()
{
    if (m_images.count() > 0) {
        m_images.clear();
        m_changed = true;
    }
}

void Slideshow::sort()
{
    if (m_comparator && m_images.count()) {
        qSort(m_images.begin(), m_images.end(), m_comparator);
        m_changed = true;
    }
}

bool Slideshow::hasCorruptedImages() const
{
    // TODO
    return true;
}

void Slideshow::removeAllCorruptedImages()
{
    for (int i = m_images.count() - 1; i >= 0; --i) {
        if (!QFile::exists(m_images.at(i).path()))
            m_images.removeAt(i);
    }
    m_changed = true;
}

bool Slideshow::sortNameAsc(const SlideshowImage &a, const SlideshowImage &b)
{
    return a.name() < b.name();
}

bool Slideshow::sortNameDesc(const SlideshowImage &a, const SlideshowImage &b)
{
    return a.name() > b.name();
}

bool Slideshow::sortDateAsc(const SlideshowImage &a, const SlideshowImage &b)
{
    return a.fileInfo().created() < b.fileInfo().created();
}

bool Slideshow::sortDateDesc(const SlideshowImage &a, const SlideshowImage &b)
{
    return a.fileInfo().created() > b.fileInfo().created();
}
