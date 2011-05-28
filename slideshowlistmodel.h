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

/*!
  \brief The SlideshowListModel class provides a model of multiple slideshows
         which can be presented to the user with the SlideshowListView class.
  \sa SlideshowListView
 */
class SlideshowListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    /*!
      \brief Constructs a new SlideshowListModel with the given \a parent.
      \param parent The parent widget.
     */
    explicit SlideshowListModel(QObject *parent = 0);

    /*!
      \brief Returns the item flags for the given \a index.
      \param index The QModelIndex of the item.
      \return The item flags.
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /*!
      \brief Returns the row count.
      \return The row count.
     */
    inline int rowCount(const QModelIndex & = QModelIndex()) const { return m_slideshowList.count(); }

    /*!
      \brief Returns the data stored under the given \a role for the item
             referred to by the \a index.
      \param index The QModelIndex of the item.
      \param role The role of the item.
      \return The stored data.
      \sa setData()
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /*!
      \brief Sets the \a role data for the item at \a index to \a value.
      \param index The QModelIndex of the item.
      \param value The data of the item.
      \param role The role of the item.
      \return \c True if successful; otherwise \c false.
      \sa data(), slideshowRenamed()
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    /*!
      \brief Returns the slideshow referred to by the \a index or \c NULL if
             the \a index is invalid or out of bounds.
      \param index The QModelIndex of the slideshow item.
      \return The slideshow or \c NULL.
      \sa slideshowList(), currentSlideshow()
     */
    Slideshow* slideshow(const QModelIndex &index) const;

    /*!
      \brief Returns the slideshow referred to by the \a name or \c NULL if
             the there is no slideshow with that \a name.
      \param name The name of the slideshow item.
      \return The slideshow or \c NULL.
      \sa slideshowList(), currentSlideshow()
     */
    Slideshow* slideshow(const QString &name) const;

    /*!
     \brief Returns the internal list of all slideshows stored in this model.
     \return The list of all slieshows.
     \sa slideshow()
     */
    inline QList<Slideshow>& slideshowList() { return m_slideshowList; }

    /*!
     \brief Returns the currently active slideshow in this model or \c NULL if
            there is no active slideshow.
     \return The current slieshow or \c NULL.
     \sa currentSlideshowIndex(), slideshow(), slideshowList()
     */
    inline Slideshow* currentSlideshow() const { return m_currentSlideshow; }

    /*!
     \brief Returns the currently active slideshow index in this model or
            an invalid model index if there is no active slideshow.
     \return The current slieshow or \c NULL.
     \sa currentSlideshow(), slideshow(), slideshowList()
     */
    inline const QModelIndex& currentSlideshowIndex() const { return m_currentSlideshowIndex; }

signals:

    /*!
      \brief This signal is emitted after a slideshow has been renamed.
      \param oldName The name before the slideshow has been renamed.
      \param newName The name after the slideshow has been renamed.
      \sa setData()
     */
    void slideshowRenamed(const QString &oldName, const QString &newName);

public slots:

    /*!
      \brief Adds a slideshow to this model.
      \param slideshow The slideshow to add.
      \return The model index of the inserted slideshow.
      \sa removeSlideshow(), clear()
     */
    QModelIndex addSlideshow(const Slideshow &slideshow);

    /*!
      \brief Removes a slideshow from this model.
      \param index The model index of the slideshow to remove.
      \return \c True if the slideshow was removed from this model;
              otherwise \c false.
      \sa addSlideshow(), clear()
     */
    bool removeSlideshow(const QModelIndex &index);

    /*!
      \brief Removes all slideshows from this model.
      \sa addSlideshow(), removeSlideshow()
     */
    void clear();

    /*!
      \brief Convenience method to add an image to the active slideshow.
      \param image The slideshow image to add.
      \return The index of the inserted image or \c -1 if the image could not
              be added (usually because there is no active slideshow).
      \sa currentSlideshow(), Slideshow::addImage(), removeImage()
     */
    int addImage(const SlideshowImage &image);

    /*!
      \brief Convenience method to remove an image from the active slideshow.
      \param index The model index of the image to remove.
      \sa currentSlideshow(), Slideshow::removeImage(), addImage()
     */
    void removeImage(const QModelIndex &index);

    /*!
      \brief Sets a slideshow referred to by the \a index to the active one.
      \param index The model index of the slideshow.
     */
    bool setCurrentSlideshow(const QModelIndex &index);

private:

    QList<Slideshow> m_slideshowList;    //!< The list of all slideshows in this model.
    Slideshow *m_currentSlideshow;       //!< The active slieshow in this model.
    QModelIndex m_currentSlideshowIndex; //!< The index of the active slideshow in this model.

};

#endif // SLIDESHOWLISTMODEL_H
