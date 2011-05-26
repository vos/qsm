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

#ifndef SLIDESHOWLISTMODEL_H
#define SLIDESHOWLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "slideshow.h"

class SlideshowListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SlideshowListModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    inline int rowCount(const QModelIndex & = QModelIndex()) const { return m_slideshowList.count(); }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Slideshow* slideshow(const QModelIndex &index) const;
    Slideshow* slideshow(const QString &name) const;

    inline QList<Slideshow>& slideshowList() { return m_slideshowList; }
    inline Slideshow* currentSlideshow() const { return m_currentSlideshow; }
    inline const QModelIndex& currentSlideshowIndex() const { return m_currentSlideshowIndex; }

signals:
    void slideshowRenamed(const QString &oldName, const QString &newName);

public slots:
    QModelIndex addSlideshow(const Slideshow &slideshow);
    bool removeSlideshow(const QModelIndex &index);
    void clear();
    int addImage(const SlideshowImage &image);
    void removeImage(const QModelIndex &index);
    bool setCurrentSlideshow(const QModelIndex &index);

private:
    QList<Slideshow> m_slideshowList;
    Slideshow *m_currentSlideshow;
    QModelIndex m_currentSlideshowIndex;
};

#endif // SLIDESHOWLISTMODEL_H
