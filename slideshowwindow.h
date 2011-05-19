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
    explicit SlideshowWindow(Slideshow *slideshow, int interval = 5, QWidget *parent = 0);
    ~SlideshowWindow();

    ImageWidget* imageWidget() const;

private slots:
    void on_imageWidget_initialized();
    void timer_timeout();
    void imageLoaded(const QImage &image, int width, int height, int index);

private:
    Ui::SlideshowWindow *ui;
    Slideshow *m_slideshow;
    SlideshowImage *m_nextImage;
    QImage m_imageBuffer;
    int m_slideshowIndex;
    int m_interval;
    QTimer m_timer;

    bool prepareNextImage(int delta = 1, bool synchronous = false);
    void showNextImage();

    void keyPressEvent(QKeyEvent *event);
};

#endif // SLIDESHOWWINDOW_H
