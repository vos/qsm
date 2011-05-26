/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <alexander.vos@stud.fh-swf.de>,
**                    Till Althaus <till.althaus@stud.fh-swf.de>
**
** This file is part of Qt Slideshow Manager (QSM).
**
** QSM is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** QSM is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with QSM.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

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
    int moveImage(int index, int delta);
    void removeImage(int index);
    inline int imageCount() const { return m_images.count(); }
    void clearImages();

    void sort();

    bool hasCorruptedImages() const;
    void removeAllCorruptedImages();

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
