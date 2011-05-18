#include "slideshowwindow.h"
#include "ui_slideshowwindow.h"

#include <QKeyEvent>

SlideshowWindow::SlideshowWindow(Slideshow *slideshow, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlideshowWindow),
    m_slideshow(slideshow)
{
    Q_ASSERT(slideshow);
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("QSM - " + slideshow->name());
    //connect(m_slideshow, SIGNAL(destroyed()), SLOT(close()));
    m_imageCount = 0;
}

SlideshowWindow::~SlideshowWindow()
{
    delete ui;
}

ImageWidget* SlideshowWindow::imageWidget() const
{
    return ui->imageWidget;
}

void SlideshowWindow::nextImage()
{
    SlideshowImage *image = m_slideshow->image(m_imageCount++);
    if (!image) return;
    if (m_imageCount >= m_slideshow->imageCount())
        m_imageCount = 0;
    ui->imageWidget->setImage(QImage(image->path()));
    ui->imageWidget->setText(image->comment());
}

void SlideshowWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Right:
        nextImage();
        break;
    }
}
