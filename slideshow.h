#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QString>
#include <QList>

#include "qsm.h"
#include "slideshowimage.h"

class Slideshow
{
public:
    static const QString FILE_EXTENSION;

    Slideshow(const QString &name = QString(), Qsm::SortFlags sort = Qsm::Unsorted, const QString &comment = QString());
    Slideshow(const Slideshow &slideshow);

    Slideshow& operator =(const Slideshow &image);

    inline const QString& name() const { return m_name; }
    void setName(const QString &name);
    QString path(const QString &dir) const;
    inline Qsm::SortFlags sort() const { return m_sort; }
    void setSort(Qsm::SortFlags sort = Qsm::Unsorted);
    inline const QString& comment() const { return m_comment; }
    void setComment(const QString &comment = QString());

    void addImage(const SlideshowImage &image);
    SlideshowImage* image(int index);
    inline QList<SlideshowImage>& images() { return m_images; }
    void removeImage(int index);
    void removeImage(const SlideshowImage &image);
    inline int imageCount() const { return m_images.count(); }
    void clearImages();

    inline void resetChanged() { m_changed = false; }
    inline bool hasChanged() const { return m_changed; }

private:
    QString m_name;
    Qsm::SortFlags m_sort;
    QString m_comment;
    QList<SlideshowImage> m_images;
    bool m_changed;
};

#endif // SLIDESHOW_H
