#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QGLWidget(parent)
{
    setMinimumSize(64, 64);

    m_imageWidth = 0;
    m_imageHeight = 0;
    m_imageAspectRatio = 1.0f;
}

void ImageWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    GLuint textureIds[1];
    glGenTextures(1, textureIds);

    glBindTexture(GL_TEXTURE_2D, textureIds[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ImageWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    m_widgetAspectRatio = (float)width / (float)height;

    updateProjection();
}

void ImageWidget::updateProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (m_widgetAspectRatio < m_imageAspectRatio) {
        int projectionHeight = int(m_imageWidth / m_widgetAspectRatio);
        int top = (m_imageHeight - projectionHeight) >> 1;
        gluOrtho2D(0, m_imageWidth, projectionHeight + top, top);
    } else if (m_widgetAspectRatio > m_imageAspectRatio) {
        int projectionWidth = int(m_imageHeight * m_widgetAspectRatio);
        int left = (m_imageWidth - projectionWidth) >> 1;
        gluOrtho2D(left, projectionWidth + left, m_imageHeight, 0);
    } else
        gluOrtho2D(0, m_imageWidth, m_imageWidth, 0);

    glMatrixMode(GL_MODELVIEW);
    updateGL();
}

void ImageWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
        glVertex2s(0, 0);
        glTexCoord2f(0.0f, 0.0f);

        glVertex2s(0, m_imageHeight);
        glTexCoord2f(1.0f, 0.0f);

        glVertex2s(m_imageWidth, m_imageHeight);
        glTexCoord2f(1.0f, 1.0f);

        glVertex2s(m_imageWidth, 0);
        glTexCoord2f(0.0f, 1.0f);
    glEnd();

    glFlush();
}

void ImageWidget::setImage(const QImage &image)
{
    if (image.isNull())
        return;

    QImage texture = QGLWidget::convertToGLFormat(image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.constBits());

    if (image.width() != m_imageWidth || image.height() != m_imageHeight) {
        m_imageWidth = image.width();
        m_imageHeight = image.height();
        m_imageAspectRatio = (float)m_imageWidth / (float)m_imageHeight;
        updateProjection();
    } else
        updateGL();
}
