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
