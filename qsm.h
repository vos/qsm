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

#ifndef QSM_H
#define QSM_H

#include <Qt>

/*!
  \brief The Qsm namespace contains miscellaneous identifiers used throughout
         different classes of the Qt Slideshow Manager.
 */
namespace Qsm
{

    /*!
      \brief This enum describes the sort options available to lists of images
             like slideshows and other presentations of multiple images. The
             flags can be combined to provide more sort options.
     */
    enum SortFlag {
        Name = 0x1,                     //!< Sort by name.
        Date = 0x2,                     //!< Sort by date.
        Unsorted = 0x4,                 //!< Do not sort.
        Reversed = 0x8,                 //!< Reverse sort order.
        NameReversed = Name | Reversed, //!< Sort by name reversed.
        DateReversed = Date | Reversed  //!< Sort by date reversed.
    };
    Q_DECLARE_FLAGS(SortFlags, SortFlag)

}

Q_DECLARE_OPERATORS_FOR_FLAGS(Qsm::SortFlags)

#endif // QSM_H
