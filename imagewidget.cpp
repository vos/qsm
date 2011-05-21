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
    m_overlayTimer.setSingleShot(true);
    connect(&m_overlayTimer, SIGNAL(timeout()), SLOT(setOverlayText()));
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
        m_translate.setY(y);
        m_zoom = (double)w / imageWidth;
    } else {
        x = m_translate.x();
        y = m_translate.y();
        w = imageWidth * m_zoom;
        h = imageHeight * m_zoom;
    }
    m_actualImageWidth = w;
    m_actualImageHeight = h;

    glViewport(x, y, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, imageWidth, 0, imageHeight, -1.0, 1.0);

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
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);

        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(m_imageWidth, 0);

        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(m_imageWidth, m_imageHeight);

        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(0, m_imageHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

    QPainter painter(this);
    if (m_textVisible && !m_text.isEmpty())
        drawText(&painter);
    if (!m_overlayText.isEmpty())
        drawOverlay(&painter);
    painter.end();

    emit viewChanged();
}

void ImageWidget::drawText(QPainter *painter)
{
    QFontMetrics metrics = QFontMetrics(m_textFont);
    int border = qMax(4, metrics.leading());
    QRect rect = metrics.boundingRect(0, 0, width() - 2 * border, int(height() * 0.25),
                                      Qt::AlignCenter | Qt::TextWordWrap, m_text);
    painter->setFont(m_textFont);
    painter->setRenderHint(QPainter::TextAntialiasing);
    int rectHeight = rect.height() + 2 * border;
    painter->fillRect(0, height() - rectHeight, width(), rectHeight,
                      m_textBackgroundColor);
    painter->setPen(m_textColor);
    painter->drawText((width() - rect.width()) / 2, height() - rectHeight + border,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, m_text);
}

void ImageWidget::drawOverlay(QPainter *painter)
{
    static const QColor bgColor = QColor(127, 127, 127, 192);
    static const QFont txtFont = QFont("", 11, QFont::DemiBold);
    static const QColor txtColor = Qt::white;
    static const int borderX = 8;
    static const int borderY = 6;
    static const QFontMetrics metrics = QFontMetrics(txtFont);

    QRect rect = metrics.boundingRect(m_overlayText);
    painter->setFont(txtFont);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(Qt::transparent);
    painter->setBrush(bgColor);
    painter->drawRoundedRect(width() / 2 - rect.width() / 2 - borderX,
                             height() / 2 - metrics.ascent() / 2 - borderY,
                             rect.width() + 2 * borderX,
                             metrics.ascent() + 2 * borderY,
                             8.0, 8.0);
    painter->setPen(txtColor);
    painter->drawText(0, 0, width(), height(), Qt::AlignCenter, m_overlayText);
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastPos = event->pos();
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
        translate(d.x(), -d.y());
        m_lastPos = event->pos();
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

void ImageWidget::setOverlayText(const QString &text, int hideAfter)
{
    m_overlayText = text;
    update();
    if (!text.isEmpty() && hideAfter > 0)
        m_overlayTimer.start(hideAfter);
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

void ImageWidget::translateTo(const QPoint &pos)
{
    m_imageMode = ManualAdjustment;
    m_translate = pos;
    setOverlayText(QString("x : %1, y : %2").arg(m_translate.x()).arg(m_translate.y()));
}

void ImageWidget::zoomTo(double factor)
{
    m_imageMode = ManualAdjustment;
    double relativeZoomFactor = factor / m_zoom;
    int dx = int(m_actualImageWidth * relativeZoomFactor) - m_actualImageWidth;
    int dy = int(m_actualImageHeight * relativeZoomFactor) - m_actualImageHeight;
    m_translate.rx() -= dx / 2;
    m_translate.ry() -= dy / 2;
    m_zoom = factor;
    setOverlayText(QString::number(int(m_zoom * 100)) + '%');
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
        m_translate.ry() -= relativeAspectChange;
    }
    setOverlayText(QString::number(int(m_rotate)) + '°');
}
