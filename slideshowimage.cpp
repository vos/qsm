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

#include "slideshowimage.h"

#include <QFileInfo>

SlideshowImage::SlideshowImage(const QString &path, int randomFactor, const QString &comment)
    : m_fileInfo(QFileInfo(path)), m_randomFactor(randomFactor), m_comment(comment)
{
}

SlideshowImage::SlideshowImage(const SlideshowImage &image)
{
    m_fileInfo = image.m_fileInfo;
    m_randomFactor = image.m_randomFactor;
    m_comment = image.m_comment;
}

SlideshowImage& SlideshowImage::operator =(const SlideshowImage &image)
{
    if (this != &image) {
        m_fileInfo = image.m_fileInfo;
        m_randomFactor = image.m_randomFactor;
        m_comment = image.m_comment;
    }
    return *this;
}

bool SlideshowImage::operator ==(const SlideshowImage &image)
{
    return m_fileInfo == image.m_fileInfo;
}
