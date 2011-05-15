#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_backgroundColor = Qt::black;
    m_textureId = 0;
    m_imageWidth = 0;
    m_imageHeight = 0;
    m_zoomMode = FitToWidget;
    m_textBackgroundColor = QColor(0, 0, 0, 127);
    m_textColor = Qt::white;
    m_textVisible = true;
}

ImageWidget::~ImageWidget()
{
    if (m_textureId > 0)
        deleteTexture(m_textureId);
}

void ImageWidget::initializeGL()
{
    qglClearColor(m_backgroundColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void ImageWidget::setupViewport(int width, int height)
{
    GLint x = 0, y = 0;
    GLsizei w = width, h = height;

    if (m_zoomMode == FitToWidget) {
        double wr = (double)width / m_imageWidth;
        double hr = (double)height / m_imageHeight;
        if (wr < hr) { // full width
            h = int(wr * m_imageHeight);
            y = (height - h) / 2;
        } else if (wr > hr) { // full height
            w = int(hr * m_imageWidth);
            x = (width - w) / 2;
        }
    } else {
        // TODO
    }

    glViewport(x, y, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_imageWidth, m_imageHeight, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
}

void ImageWidget::paintEvent(QPaintEvent *)
{
    makeCurrent();
    setupViewport(width(), height());

    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f);

        glVertex2i(0, m_imageHeight);
        glTexCoord2f(1.0f, 0.0f);

        glVertex2i(m_imageWidth, m_imageHeight);
        glTexCoord2f(1.0f, 1.0f);

        glVertex2i(m_imageWidth, 0);
        glTexCoord2f(0.0f, 1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

    QPainter painter(this);
    if (m_textVisible && !m_text.isEmpty())
        drawText(&painter, m_text);
    painter.end();

    glFlush();
}

void ImageWidget::drawText(QPainter *painter, const QString &text)
{
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2 * border, int(height() * 0.25),
                                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    int rectHeight = rect.height() + 2 * border;
    painter->fillRect(0, height() - rectHeight, width(), rectHeight,
                      m_textBackgroundColor);
    painter->setPen(m_textColor);
    painter->drawText((width() - rect.width()) / 2, height() - rectHeight + border,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, text);
}

void ImageWidget::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    qglClearColor(color);
    update();
}

void ImageWidget::setImage(const QImage &image)
{
    if (image.isNull())
        return;

    m_textureId = bindTexture(image);
    m_imageWidth = image.width();
    m_imageHeight = image.height();
    update();
}

void ImageWidget::setZoomMode(ImageWidget::ZoomMode zoomMode)
{
    m_zoomMode = zoomMode;
    update();
}

void ImageWidget::setText(const QString &text)
{
    m_text = text;
    update();
}

void ImageWidget::setTextBackgroundColor(const QColor &color)
{
    m_textBackgroundColor = color;
    update();
}

void ImageWidget::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

void ImageWidget::setTextVisible(bool visible)
{
    m_textVisible = visible;
    update();
}
