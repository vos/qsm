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

#ifndef SLIDESHOWIMAGE_H
#define SLIDESHOWIMAGE_H

#include <QString>
#include <QFileInfo>

class SlideshowImage
{
public:
    SlideshowImage(const QString &path, int randomFactor = 1, const QString &comment = QString());
    SlideshowImage(const SlideshowImage &image);

    SlideshowImage& operator =(const SlideshowImage &image);
    bool operator ==(const SlideshowImage &image);

    inline const QFileInfo& fileInfo() const { return m_fileInfo; }
    inline QString path() const { return m_fileInfo.absoluteFilePath(); }
    inline QString name() const { return m_fileInfo.fileName(); }
    inline int randomFactor() const { return m_randomFactor; }
    inline const QString& comment() const { return m_comment; }

    inline void setPath(const QString &path) { m_fileInfo.setFile(path); }
    inline void setRandomFactor(int randomFactor = 1) { m_randomFactor = randomFactor; }
    inline void setComment(const QString &comment = QString()) { m_comment = comment; }

private:
    QFileInfo m_fileInfo;
    int m_randomFactor;
    QString m_comment;
};

#endif // SLIDESHOWIMAGE_H
