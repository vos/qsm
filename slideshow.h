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

    Slideshow(const QString &name = QString(), Qsm::SortFlags sortFlags = Qsm::Unsorted, const QString &comment = QString());
    Slideshow(const Slideshow &slideshow);

    Slideshow& operator =(const Slideshow &image);

    inline const QString& name() const { return m_name; }
    void setName(const QString &name);
    QString path(const QString &dir) const;
    inline Qsm::SortFlags sortFlags() const { return m_sort; }
    bool setSortFlags(Qsm::SortFlags sortFlags = Qsm::Unsorted);
    inline const QString& comment() const { return m_comment; }
    void setComment(const QString &comment = QString());

    int addImage(const SlideshowImage &image);
    SlideshowImage* image(int index);
    inline QList<SlideshowImage>& images() { return m_images; }
    void moveImage(int from, int delta);
    void removeImage(int index);
    inline int imageCount() const { return m_images.count(); }
    void clearImages();

    void sort();

    inline void setChanged() { m_changed = true; }
    inline void resetChanged() { m_changed = false; }
    inline bool hasChanged() const { return m_changed; }

private:
    static bool sortNameAsc(const SlideshowImage &a, const SlideshowImage &b);
    static bool sortNameDesc(const SlideshowImage &a, const SlideshowImage &b);
    static bool sortDateAsc(const SlideshowImage &a, const SlideshowImage &b);
    static bool sortDateDesc(const SlideshowImage &a, const SlideshowImage &b);

    QString m_name;
    Qsm::SortFlags m_sort;
    bool (*m_comparator)(const SlideshowImage &a, const SlideshowImage &b);
    QString m_comment;
    QList<SlideshowImage> m_images;
    bool m_changed;
};

#endif // SLIDESHOW_H
