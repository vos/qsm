#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QGLWidget>
#include <QImage>

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif

class ImageWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);

    void setImage(const QImage &image);

private:
    float m_widgetAspectRatio;
    int m_imageWidth;
    int m_imageHeight;
    float m_imageAspectRatio;

    void initializeGL();
    void resizeGL(int width, int height);
    void updateProjection();
    void paintGL();

};

#endif // IMAGEWIDGET_H
