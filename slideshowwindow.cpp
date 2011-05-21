#include "slideshowwindow.h"
#include "ui_slideshowwindow.h"

#include <QKeyEvent>
#include <QThreadPool>

#include "imageloader.h"

SlideshowWindow::SlideshowWindow(Slideshow *slideshow, int interval, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlideshowWindow),
    m_slideshow(slideshow),
    m_nextImage(NULL),
    m_slideshowIndex(0),
    m_interval(interval)
{
    Q_ASSERT(slideshow);
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("QSM - " + slideshow->name());
    //connect(m_slideshow, SIGNAL(destroyed()), SLOT(close()));
    connect(&m_timer, SIGNAL(timeout()), SLOT(timer_timeout()));
}

void SlideshowWindow::on_imageWidget_initialized()
{
    if (prepareNextImage(0, true)) {
        showNextImage();
        if (prepareNextImage())
            m_timer.start(m_interval * 1000);
    } else
        close();
}

SlideshowWindow::~SlideshowWindow()
{
    delete ui;
}

ImageWidget* SlideshowWindow::imageWidget() const
{
    return ui->imageWidget;
}

bool SlideshowWindow::prepareNextImage(int delta, bool synchronous)
{
    int index = m_slideshowIndex + delta;
    if (index < 0 || index >= m_slideshow->imageCount())
        return false;
    if (m_nextImage = m_slideshow->image(index)) {
        if (synchronous) {
            m_imageBuffer = QImage(m_nextImage->path());
            m_slideshowIndex = index;
        } else {
            ImageLoader *imageLoader = new ImageLoader(m_nextImage->path(), index);
            connect(imageLoader, SIGNAL(imageLoaded(QImage, int, int, int)), SLOT(imageLoaded(QImage, int, int, int)));
            QThreadPool::globalInstance()->start(imageLoader, 1);
        }
    } else
        return false;
    return true;
}

void SlideshowWindow::showNextImage()
{
    if (!m_nextImage || m_imageBuffer.isNull())
        return;
    ui->imageWidget->setImage(m_imageBuffer);
    ui->imageWidget->setText(m_nextImage->comment());
}

void SlideshowWindow::timer_timeout()
{
    showNextImage();
    if (!prepareNextImage())
        m_timer.stop();
}

void SlideshowWindow::imageLoaded(const QImage &image, int, int, int index)
{
    m_imageBuffer = image;
    m_slideshowIndex = index;
}

void SlideshowWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_0:
        ui->imageWidget->setImageMode();
        break;
    case Qt::Key_1:
        ui->imageWidget->zoomTo();
        break;
    case Qt::Key_R:
        if (event->modifiers() & Qt::ShiftModifier)
            ui->imageWidget->rotateCounterclockwise();
        else
            ui->imageWidget->rotateClockwise();
        break;
    case Qt::Key_Plus:
        ui->imageWidget->zoomIn();
        break;
    case Qt::Key_Minus:
        ui->imageWidget->zoomOut();
        break;
    case Qt::Key_C:
        ui->imageWidget->toggleTextVisibility();
        break;
    case Qt::Key_Space:
        if (m_timer.isActive()) {
            m_timer.stop();
            ui->imageWidget->setOverlayText(tr("Slideshow paused"), 2000);
        } else if (prepareNextImage()) {
            m_timer.start();
            ui->imageWidget->setOverlayText(tr("Slideshow continued"), 2000);
        }
        break;
    case Qt::Key_Right:
        if (m_timer.isActive())
            m_timer.stop();
        if (prepareNextImage(1, true))
            showNextImage();
        break;
    case Qt::Key_Left:
        if (m_timer.isActive())
            m_timer.stop();
        if (prepareNextImage(-1, true))
            showNextImage();
        break;
    }
}
