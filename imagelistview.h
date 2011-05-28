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

#ifndef IMAGELISTVIEW_H
#define IMAGELISTVIEW_H

#include <QListView>

/*!
  \brief The ImageListView class provides a list view onto a ImageListModel.
  \sa ImageListModel
 */
class ImageListView : public QListView
{
    Q_OBJECT

public:

    /*!
      \brief Creates a new ImageListView with the given \a parent to view
             a ImageListModel.
      \param parent The parent widget.
     */
    explicit ImageListView(QWidget *parent = 0);

    /*!
      \brief Returns a list of all selected and non-hidden item indexes in the view.
      \return A list of all selected and non-hidden item indexes.
     */
    inline QModelIndexList selectedIndexes() const { return QListView::selectedIndexes(); }

};

#endif // IMAGELISTVIEW_H
