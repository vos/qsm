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

#ifndef SLIDESHOWLISTVIEW_H
#define SLIDESHOWLISTVIEW_H

#include <QListView>

class SlideshowListView : public QListView
{
    Q_OBJECT

public:
    explicit SlideshowListView(QWidget *parent = 0);

signals:
    void selectionChanged(const QModelIndex &index);

protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

};

#endif // SLIDESHOWLISTVIEW_H
