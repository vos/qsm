#ifndef SLIDESHOWWINDOW_H
#define SLIDESHOWWINDOW_H

#include <QWidget>

#include "slideshow.h"

class ImageWidget;

namespace Ui {
    class SlideshowWindow;
}

class SlideshowWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SlideshowWindow(Slideshow *slideshow, QWidget *parent = 0);
    ~SlideshowWindow();

    ImageWidget* imageWidget() const;

private:
    Ui::SlideshowWindow *ui;
    Slideshow *m_slideshow;
    int m_imageCount; // test

    void nextImage();
    void keyPressEvent(QKeyEvent *event);
};

#endif // SLIDESHOWWINDOW_H
