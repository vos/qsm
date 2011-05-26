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

#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QFileInfo>
#include <QIcon>

class ImageInfo
{
public:
    ImageInfo(const QFileInfo &fileInfo = QFileInfo());
    ImageInfo(const ImageInfo &obj);

    ImageInfo& operator =(const ImageInfo &obj);

    inline const QFileInfo& fileInfo() const { return m_fileInfo; }
    inline QFileInfo& fileInfo() { return m_fileInfo; }
    inline QString imagePath() const { return m_fileInfo.absoluteFilePath(); }
    inline bool exists() const { return m_fileInfo.exists(); }
    inline const QIcon& icon() const { return m_icon; }
    inline int width() const { return m_width; }
    inline int height() const { return m_height; }
    QString dimensions() const;
    QString size() const;

    inline void setIcon(const QIcon &icon) { m_icon = icon; }
    void setDimensions(int width, int height);

private:
    static QIcon DEFAULT_ICON;

    QFileInfo m_fileInfo;
    QIcon m_icon;
    int m_width;
    int m_height;
};

#endif // IMAGEINFO_H
