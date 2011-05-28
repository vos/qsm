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

/*!
  \brief The SlideshowListView class provides a list view onto a SlideshowListModel.
  \sa SlideshowListModel, Slideshow
 */
class SlideshowListView : public QListView
{
    Q_OBJECT

public:

    /*!
      \brief Creates a new SlideshowListView with the given \a parent to view
             a SlideshowListModel.
      \param parent The parent widget.
     */
    explicit SlideshowListView(QWidget *parent = 0);

signals:

    /*!
      \brief This signal is emitted after the selection of the view has been changed.
      \param index The model index of the selected item.
     */
    void selectionChanged(const QModelIndex &index);

protected slots:

    /*!
      \brief Changes the selection of the view and emits the selectionChanged() signal.
      \param selected The item selection of the selected item.
      \param deselected The item selection of the deselected item.
      \sa selectionChanged()
     */
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

};

#endif // SLIDESHOWLISTVIEW_H
