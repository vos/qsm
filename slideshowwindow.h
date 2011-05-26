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

#ifndef SLIDESHOWWINDOW_H
#define SLIDESHOWWINDOW_H

#include <QWidget>
#include <QTimer>

#include "slideshow.h"

class ImageWidget;

namespace Ui {
    class SlideshowWindow;
}

class SlideshowWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SlideshowWindow(Slideshow *slideshow, int interval = 5, bool random = false, bool repeat = true, QWidget *parent = 0);
    ~SlideshowWindow();

    ImageWidget* imageWidget() const;

private slots:
    void on_imageWidget_initialized();
    void on_imageWidget_doubleClicked();
    void timer_timeout();
    void timer_stop();
    void imageLoaded(const QImage &image, int width, int height, int index);

private:
    Ui::SlideshowWindow *ui;
    Slideshow *m_slideshow;
    bool m_random;
    bool m_repeat;
    QVector<double> m_randomFactors;
    SlideshowImage *m_currentImage;
    SlideshowImage *m_nextImage;
    QImage m_imageBuffer;
    int m_slideshowIndex;
    QTimer m_timer;

    inline bool hasNextImage() const { return (m_nextImage && m_nextImage != m_currentImage); }
    bool prepareNextImage(int delta = 1, bool synchronous = false);
    void showNextImage();

    void keyPressEvent(QKeyEvent *event);
};

#endif // SLIDESHOWWINDOW_H
