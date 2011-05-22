#include "slideshowwindow.h"
#include "ui_slideshowwindow.h"

#include <QKeyEvent>
#include <QThreadPool>
#include <QTime>

#include "imageloader.h"

SlideshowWindow::SlideshowWindow(Slideshow *slideshow, int interval, bool random, bool repeat, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlideshowWindow),
    m_slideshow(slideshow),
    m_random(random),
    m_repeat(repeat),
    m_currentImage(NULL),
    m_nextImage(NULL),
    m_slideshowIndex(0)
{
    Q_ASSERT(slideshow);
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("QSM - " + slideshow->name());
    m_timer.setInterval(interval * 1000);
    connect(&m_timer, SIGNAL(timeout()), SLOT(timer_timeout()));
    //connect(m_slideshow, SIGNAL(destroyed()), SLOT(close()));

    if (random) {
        // initialize random generator
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        // initialize random factors
        m_randomFactors = QVector<double>(slideshow->imageCount());
        int i = 0;
        int sum = 0;
        foreach (const SlideshowImage &image, slideshow->images()) {
            int factor = image.randomFactor();
            m_randomFactors[i++] = factor;
            sum += factor;
        }
        // normalize random factors
        for (i = 0; i < m_randomFactors.count(); ++i)
            m_randomFactors[i] /= sum;
    }
}

void SlideshowWindow::on_imageWidget_initialized()
{
    ui->imageWidget->setOverlayText(m_slideshow->name(), m_timer.interval());
    if (prepareNextImage(0, true)) {
        showNextImage();
        if (m_nextImage->comment().isEmpty() && !m_slideshow->comment().isEmpty())
            ui->imageWidget->setText(m_slideshow->comment());
        if (prepareNextImage())
            m_timer.start();
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
    int index;
    if (m_random) {
        // get next index based on the weighted random algorithm
        double rnd = qrand() / (double)RAND_MAX;
        double sum = 0.0;
        for (index = 0; index < m_randomFactors.count(); ++index) {
            sum += m_randomFactors[index];
            if (sum >= rnd)
                break;
        }
    } else {
        // get next index depending on delta
        index = m_slideshowIndex + delta;
        if (index < 0) {
            if (m_repeat)
                index = m_slideshow->imageCount() - 1;
            else {
                m_nextImage = NULL;
                return false;
            }
        } else if (index >= m_slideshow->imageCount()) {
            if (m_repeat)
                index = 0;
            else {
                m_nextImage = NULL;
                return false;
            }
        }
    }
    // set next image and validate it
    if ((m_nextImage = m_slideshow->image(index))) {
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
    m_currentImage = m_nextImage;
}

void SlideshowWindow::timer_timeout()
{
    showNextImage();
    if (!prepareNextImage())
        timer_stop();
}

void SlideshowWindow::timer_stop()
{
    if (m_timer.isActive()) {
        m_timer.stop();
        ui->imageWidget->setOverlayText(tr("Slideshow stopped"), 2000);
    }
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
        ui->imageWidget->setOverlayText(tr("Comments %1").arg(ui->imageWidget->toggleTextVisibility() ? tr("enabled") : tr("disabled")));
        break;
    case Qt::Key_Space:
        if (m_timer.isActive())
            timer_stop();
        else if (hasNextImage() || prepareNextImage()) {
            m_timer.start();
            ui->imageWidget->setOverlayText(tr("Slideshow continued"), 2000);
        }
        break;
    case Qt::Key_Right:
        timer_stop();
        if (hasNextImage() || prepareNextImage(1, true))
            showNextImage();
        break;
    case Qt::Key_Left:
        timer_stop();
        if (prepareNextImage(hasNextImage() ? -2 : -1, true))
            showNextImage();
        break;
    }
}
