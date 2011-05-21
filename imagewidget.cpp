#include "imagewidget.h"

#include <QMouseEvent>

ImageWidget::ImageWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_backgroundColor = Qt::black;
    m_textureId = 0;
    m_imageWidth = m_imageHeight = 0;
    m_actualImageWidth = m_actualImageHeight = 0;
    m_textBackgroundColor = QColor(0, 0, 0, 127);
    m_textColor = Qt::white;
    m_textVisible = true;
    m_imageMode = FitToWidget;
    m_zoom = 1.0;
    m_rotate = 0.0;
}

ImageWidget::~ImageWidget()
{
    if (m_textureId > 0)
        deleteTexture(m_textureId);
}

void ImageWidget::initializeGL()
{
    qglClearColor(m_backgroundColor);
    emit initialized();
}

void ImageWidget::setupViewport(int width, int height)
{
    GLint x = 0, y = 0;
    GLsizei w = width, h = height;

    int imageWidth, imageHeight;
    if (m_rotate == 90.0 || m_rotate == 270.0) {
        imageWidth = m_imageHeight;
        imageHeight = m_imageWidth;
    } else {
        imageWidth = m_imageWidth;
        imageHeight = m_imageHeight;
    }

    if (m_imageMode == FitToWidget) {
        double wr = (double)width / imageWidth;
        double hr = (double)height / imageHeight;
        if (wr < hr) { // full width
            h = int(wr * imageHeight);
            y = (height - h) / 2;
        } else if (wr > hr) { // full height
            w = int(hr * imageWidth);
            x = (width - w) / 2;
        }
        m_translate.setX(x);
        m_translate.setY(-y);
        m_zoom = (double)w / imageWidth;
    } else {
        x = m_translate.x();
        y = -m_translate.y();
        w = imageWidth * m_zoom;
        h = imageHeight * m_zoom;
    }
    m_actualImageWidth = w;
    m_actualImageHeight = h;

    glViewport(x, y, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, imageWidth, imageHeight, 0, -1.0, 1.0);

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

    if (m_rotate == 90.0) {
        glRotated(m_rotate, 0.0, 0.0, 1.0);
        glTranslated(0.0, -m_imageHeight, 0.0);
    } else if (m_rotate == 180.0) {
        glScaled(-1.0, -1.0, 1.0);
        glTranslated(-m_imageWidth, -m_imageHeight, 0.0);
    } else if (m_rotate == 270.0) {
        glRotated(m_rotate, 0.0, 0.0, 1.0);
        glTranslated(-m_imageWidth, 0.0, 0.0);
    }

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

    if (m_textVisible && !m_text.isEmpty()) {
        QPainter painter(this);
        drawText(&painter, m_text);
        painter.end();
    } else {
        glFlush();
        swapBuffers();
    }

    emit viewChanged();
}

void ImageWidget::drawText(QPainter *painter, const QString &text)
{
    QFontMetrics metrics = QFontMetrics(m_textFont);
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2 * border, int(height() * 0.25),
                                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter->setFont(m_textFont);
    painter->setRenderHint(QPainter::TextAntialiasing);
    int rectHeight = rect.height() + 2 * border;
    painter->fillRect(0, height() - rectHeight, width(), rectHeight,
                      m_textBackgroundColor);
    painter->setPen(m_textColor);
    painter->drawText((width() - rect.width()) / 2, height() - rectHeight + border,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, text);
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastPos = event->pos();
        m_imageMode = ManualAdjustment;
        m_cursor = cursor().shape();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else if (event->button() == Qt::MiddleButton) {
        setImageMode();
        event->accept();
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint d = event->pos() - m_lastPos;
        m_translate += d;
        m_lastPos = event->pos();
        update();
        event->accept();
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(m_cursor);
}

void ImageWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        zoomIn();
    else
        zoomOut();
    event->accept();
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

    if (m_textureId > 0)
        deleteTexture(m_textureId);

    m_textureId = bindTexture(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    m_imageWidth = image.width();
    m_imageHeight = image.height();
    setImageMode();
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

void ImageWidget::setTextFont(const QFont &font)
{
    m_textFont = font;
    update();
}

void ImageWidget::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

void ImageWidget::setTextVisibility(bool visible)
{
    m_textVisible = visible;
    update();
}

bool ImageWidget::toggleTextVisibility()
{
    m_textVisible = !m_textVisible;
    update();
    return m_textVisible;
}

void ImageWidget::setImageMode(ImageWidget::ImageMode mode)
{
    m_imageMode = mode;
    if (mode == FitToWidget) {
        m_translate = QPoint();
        m_zoom = 1.0;
        m_rotate = 0.0;
    }
    update();
}

void ImageWidget::zoomTo(double factor)
{
    m_imageMode = ManualAdjustment;
    double relativeZoomFactor = factor / m_zoom;
    int dx = int(m_actualImageWidth * relativeZoomFactor) - m_actualImageWidth;
    int dy = int(m_actualImageHeight * relativeZoomFactor) - m_actualImageHeight;
    m_translate.rx() -= dx / 2;
    m_translate.ry() += dy / 2;
    m_zoom = factor;
    update();
}

void ImageWidget::rotateTo(double angle)
{
    m_rotate = angle;
    if (m_rotate < 0.0)
        m_rotate += 360.0;
    else if (m_rotate >= 360.0)
        m_rotate -= 360.0;
    if (m_imageMode == ManualAdjustment) {
        int relativeAspectChange = qAbs(m_actualImageWidth - m_actualImageHeight) / 2;
        if (m_rotate == 0.0 || m_rotate == 180.0)
            relativeAspectChange = -relativeAspectChange;
        m_translate.rx() += relativeAspectChange;
        m_translate.ry() += relativeAspectChange;
    }
    update();
}
