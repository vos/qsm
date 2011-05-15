#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QGLWidget>
#include <QImage>

class ImageWidget : public QGLWidget
{
    Q_OBJECT

public:
    enum ZoomMode {
        FitToWidget,
        DontChange
    };

    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

public slots:
    void setBackgroundColor(const QColor &color = Qt::black);
    void setImage(const QImage &image);
    void setZoomMode(ZoomMode zoomMode = FitToWidget);
    void setText(const QString &text = QString());
    void setTextBackgroundColor(const QColor &color = QColor(0, 0, 0, 127));
    void setTextColor(const QColor &color = Qt::white);
    void setTextVisible(bool visible = true);

private:
    QColor m_backgroundColor;
    GLuint m_textureId;
    int m_imageWidth;
    int m_imageHeight;
    ZoomMode m_zoomMode;
    QString m_text;
    QColor m_textBackgroundColor;
    QColor m_textColor;
    bool m_textVisible;

    void initializeGL();
    void setupViewport(int width, int height);
    void paintEvent(QPaintEvent *event);
    void drawText(QPainter *painter, const QString &text);
};

#endif // IMAGEWIDGET_H
