#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QString>
#include <QList>

#include "qsm.h"
#include "slideshowimage.h"

class Slideshow
{
public:
    Slideshow(const QString &name, Qsm::SortFlags sort = Qsm::Name, const QString &comment = QString());
    Slideshow(const Slideshow &slideshow);

    Slideshow& operator =(const Slideshow &image);

    inline const QString& name() const { return m_name; }
    inline void setName(const QString &name) { m_name = name; }
    inline Qsm::SortFlags sort() const { return m_sort; }
    void setSort(Qsm::SortFlags sort = Qsm::Name);
    inline const QString& comment() const { return m_comment; }
    inline void setComment(const QString &comment = QString()) { m_comment = comment; }

    void addImage(const SlideshowImage &image);
    SlideshowImage* image(int index);
    inline const QList<SlideshowImage>& images() { return m_images; }
    void removeImage(int index);
    void removeImage(const SlideshowImage &image);
    inline int imageCount() const { return m_images.count(); }
    inline void clearImages() { m_images.clear(); }

private:
    QString m_name;
    Qsm::SortFlags m_sort;
    QString m_comment;
    QList<SlideshowImage> m_images;
};

#endif // SLIDESHOW_H
